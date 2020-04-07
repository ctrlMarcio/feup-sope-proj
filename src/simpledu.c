#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "simpledu.h"
#include "error.h"
#include "logger.h"
#include "ui.h"

#define READ 0
#define WRITE 1
#define MAX_SIZE 2048
#define MAX_DIR_SIZE 4096

void simpledu(struct flags *flags, int *fd);
pid_t treatDir(int *old_fd, int *new_fd, struct flags *flags, struct dirent *dirent, struct stat *stat_entry);
void closeDir(int *old_fd, int *new_fd, DIR * dir, struct flags *flags, int *total_size);

void run(struct flags *flags)
{
    int fd[2];
    pipe(fd);
    simpledu(flags, fd);
}

// Each simpledu calls correponds a directory
void simpledu(struct flags *flags, int *old_fd)
{
    struct dirent *dirent;
    struct stat stat_entry;
    DIR *dir;
    int total_size = 0;

    // opens the directory with the name given on path (always updated)
    if ((dir = opendir(flags->path)) == NULL)
    {
        perror(flags->path);
        exit(DIR_ERROR);
    }

    // opens a new pipe for subdirectories
    int new_fd[2];
    if (pipe(new_fd) == -1)
    {
        perror("Creating pipe on new directory");
        exit(PIPE_ERROR);
    }

    // runs through every entry in the directory
    while ((dirent = readdir(dir)) != NULL)
    {
        // gets the full path of the entry in the "file" string
        char file[MAX_SIZE];
        sprintf(file, "%s/%s", flags->path, dirent->d_name);
        lstat(file, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode)) // when the entry is a subdirectory
        {
            if (!strcmp(dirent->d_name, "..")) // simply ignores the ".." directory
                continue;

            // starts the conting of the directory size at the "." subidr, this is, first of all counts the size of the actual directroy
            if (!strcmp(dirent->d_name, "."))
            {
                total_size += stat_entry.st_size;
                continue;
            }

            // ignores the size of sub directories if the option to separated dirs is enabled and the max depth is reached
            if (flags->separate_dirs && flags->current_depth > flags->max_depth && flags->max_depth > 0)
            {
                continue;
            }

            // gets the size of the subdirectory recursively
            pid_t pid = treatDir(old_fd, new_fd, flags, dirent, &stat_entry);
            if (pid == 0) // treatDir returns 0 if the thread is the child, its size is written to a pipe instead
                exit(0);
        }
        else if (S_ISREG(stat_entry.st_mode)) // when the entry is a subdirectory
        {
            // updates the size and prints it if the max depth was not reached already
            total_size += stat_entry.st_size;
            if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                printFile(file, flags, stat_entry.st_size);
        }
        else if (S_ISLNK(stat_entry.st_mode)) // when the entry is a subdirectory
        {
            total_size += stat_entry.st_size;
            // TODO treat links here
            if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                printFile(file, flags, stat_entry.st_size);
        }
    }

    // closes the directory and executes complementar operations (e.g. writing to pipe and console)
    closeDir( old_fd, new_fd, dir, flags, &total_size);
}

/**
 * Creates a new process and runs the simpledu in it
 *
 * @return      the process id (will return two)
 */
pid_t treatDir(int *old_fd, int *new_fd, struct flags *flags, struct dirent *dirent, struct stat *stat_entry)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Forking");
        exit(FORK_ERROR);
    }
    else if (pid == 0)
    { // child enters inside the new directory
        struct flags tmp_flags = *flags;
        strcat(tmp_flags.path, "/");
        strcat(tmp_flags.path, dirent->d_name);

        tmp_flags.current_depth++;

        simpledu(&tmp_flags, new_fd);
    }
    else
    { // parent waits for child to end, reads its size from the new pipe, and updates the total_size
        entryLog(pid, CREATE, flags->line_args);
        //FIXME between this and read at the end of closeDir, delete if decided at the end of closeDir, there's a FIXME there
        /*int tmp;
        while (wait(NULL) > 0) {
            read(fd[READ], &tmp, sizeof(int));
            // TODO log RECV_PIPE
            // TODO log EXIT
            total_size += tmp;
        }*/
    }

    return pid;
}

/**
 * Counts the size of all subdirectories, prints the current directory information and closes it
 */
void closeDir(int *old_fd, int *new_fd, DIR *dir, struct flags *flags, int *total_size)
{
    // closes the directory
    if (closedir(dir) == -1)
    {
        perror(flags->path);
        exit(DIR_ERROR);
    }

    int status;
    int pid;

    int tmp;

    //FIXME between this and read at the end of treatDir, delete if decided at the end of treatDir, there's a FIXME there
    // waits for remaining files
    while ((pid = wait(&status)) > 0)
    {
        char line[32];
        sprintf(line, "termination code %d", WEXITSTATUS(status));
        entryLog(pid, EXIT, line);

        // reads all the sizes from the subdirs
        read(new_fd[READ], &tmp, sizeof(int));

        // if separate dirs is deactivated, counts the size to the total size
        if (!flags->separate_dirs)
            *total_size += tmp;
        // TODO log RECV_PIPE
    }

    // if the max depth was not reached already, prints the directory
    if (!(flags->current_depth - 1 > flags->max_depth && flags->max_depth > 0))
        printDir(flags->path, flags, *total_size);

    // writes its total size in the pipe so the above directory can count it
    write(old_fd[WRITE], total_size, sizeof(int));
    // TODO log SEND_PIPE
}
