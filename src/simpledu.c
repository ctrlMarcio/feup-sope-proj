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
#include "process_handler.h"

#define READ 0
#define WRITE 1
#define MAX_SIZE 2048
#define MAX_DIR_SIZE 4096
#define MAX_DIR_NAME_SIZE 512

void simpledu(struct flags *flags, int *fd);
pid_t treatDir(int *new_fd, struct flags *flags, struct dirent *dirent);
pid_t treatLink(int *new_fd, struct flags *flags, struct dirent *dirent, char *line, double *total_size);
void closeDir(int *old_fd, int *new_fd, DIR *dir, struct flags *flags, double *total_size);

void run(struct flags *flags)
{
    int fd[2];
    pipe(fd);
    setupParentHandlers();
    simpledu(flags, fd);
}

// Each simpledu calls correponds to a directory
void simpledu(struct flags *flags, int *old_fd)
{
    struct dirent *dirent;
    struct stat stat_entry;
    DIR *dir;
    double total_size = 0;

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
                if (!flags->bytes)
                {
                    total_size += stat_entry.st_blocks / 2.0;
                }
                else
                {
                    total_size += stat_entry.st_size;
                }
                continue;
            }

            // ignores the size of sub directories if the option to separated dirs is enabled and the max depth is reached
            if (flags->separate_dirs && flags->current_depth > flags->max_depth && flags->max_depth > 0)
            {
                continue;
            }

            // gets the size of the subdirectory recursively
            pid_t pid = treatDir(new_fd, flags, dirent);
            if (pid == 0) // treatDir returns 0 if the thread is the child, its size is written to a pipe instead
                exit(0);
        }
        else if (S_ISREG(stat_entry.st_mode)) // when the entry is a file
        {
            // updates the size and prints it if the max depth was not reached already
            double size;
            if (!flags->bytes)
            {
                size = stat_entry.st_blocks / 2.0;
                total_size += size;
            }
            else
            {
                size = stat_entry.st_size;
                total_size += size;
            }
            if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                printFile(dirent->d_name, flags, size);

            logEntry(flags, stat_entry.st_size, file);
        }
        else if (S_ISLNK(stat_entry.st_mode)) // when the entry is a link
        {
            // If the links flag is off the link must be treated as a file
            if (!flags->dereference)
            {
                double size;
                if (!flags->bytes)
                {
                    size = stat_entry.st_blocks / 2.0;
                    total_size += size;
                }
                else
                {
                    size = stat_entry.st_size;
                    total_size += size;
                }
                if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                    printFile(dirent->d_name, flags, size);
            }
            else
            {
                if (contains((int)stat_entry.st_ino, flags->inodes_read, flags->inodes_read_size))
                    exit(0);

                flags->inodes_read[flags->inodes_read_size++] = stat_entry.st_ino;

                pid_t pid = treatLink(new_fd, flags, dirent, file, &total_size);
                if (pid == 0) // treatDir returns 0 if the thread is the child, its size is written to a pipe instead
                    exit(0);
            }
        }
    }

    // closes the directory and executes complementar operations (e.g. writing to pipe and console)
    closeDir(old_fd, new_fd, dir, flags, &total_size);
}

/**
 * Creates a new process and runs the simpledu in it
 *
 * @return      the process id (will return two)
 */
pid_t treatDir(int *new_fd, struct flags *flags, struct dirent *dirent)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Forking");
        exit(FORK_ERROR);
    }
    else if (pid == 0)
    {                           // child enters inside the new directory
        setupProcessHandlers(); // sets the current handlers

        // sets the children group if it is not set already
        if (!childrenpg)
            childrenpg = getpid();

        // builds new path
        struct flags tmp_flags = *flags;
        strcat(tmp_flags.path, "/");
        strcat(tmp_flags.path, dirent->d_name);

        strcat(tmp_flags.link_path, "/");
        strcat(tmp_flags.link_path, dirent->d_name);

        tmp_flags.current_depth++;

        simpledu(&tmp_flags, new_fd);
    }
    else
    { // parent waits for child to end, reads its size from the new pipe, and updates the total_size
        setChildrenGroup(pid);

        logCreate(flags);
    }

    return pid;
}

/**
 * Creates a new process and runs the simpledu in it
 *
 * @return      the process id (will return two)
 */
pid_t treatLink(int *new_fd, struct flags *flags, struct dirent *dirent, char *file, double *total_size)
{
    char linkpath_buffer[MAX_DIR_NAME_SIZE];
    int linkpath_bytes = readlink(file, linkpath_buffer, MAX_DIR_NAME_SIZE);

    struct flags tmp_flags = *flags;
    strncpy(tmp_flags.link_path, tmp_flags.path, strlen(tmp_flags.path));
    strcat(tmp_flags.link_path, "/");
    strcat(tmp_flags.link_path, dirent->d_name);
    strncpy(tmp_flags.path, linkpath_buffer, linkpath_bytes);

    struct stat stat_entry;

    char line[1024];

    sprintf(line, "%s/%s", flags->path, tmp_flags.path);

    lstat(line, &stat_entry);

    if (S_ISREG(stat_entry.st_mode)) // when the entry is a file
    {
        double size;
        if (!flags->bytes)
        {
            size = stat_entry.st_blocks / 2;
            *total_size += size;
        }
        else
        {
            size = stat_entry.st_size;
            *total_size += size;
        }
        if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
        {
            printLink(file, &tmp_flags, size);
        }
        return getpid();
    }

    tmp_flags.linked = 1;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Forking");
        exit(FORK_ERROR);
    }
    else if (pid == 0)
    { // child enters inside the new directory
        setupProcessHandlers();
        if (!childrenpg)
            childrenpg = getpid();

        tmp_flags.current_depth++;

        simpledu(&tmp_flags, new_fd);
    }
    else
    { // parent waits for child to end, reads its size from the new pipe, and updates the total_size
        setChildrenGroup(pid);

        logCreate(flags);
    }

    return pid;
}

/**
 * Counts the size of all subdirectories, prints the current directory information and closes it
 */
void closeDir(int *old_fd, int *new_fd, DIR *dir, struct flags *flags, double *total_size)
{
    // closes the directory
    if (closedir(dir) == -1)
    {
        perror(flags->path);
        exit(DIR_ERROR);
    }

    int status;
    int pid;

    double tmp;

    // waits for remaining files
    while ((pid = wait(&status)) > 0)
    {
        logExit(status);
        // reads all the sizes from the subdirs
        read(new_fd[READ], &tmp, sizeof(double));

        logRecvPipe(tmp);

        // if separate dirs is deactivated, counts the size to the total size
        if (!flags->separate_dirs)
            *total_size += tmp;
    }

    // if the max depth was not reached already, prints the directory
    if (!(flags->current_depth - 1 > flags->max_depth && flags->max_depth > 0))
        printDir(flags->path, flags, *total_size);

    // writes its total size in the pipe so the above directory can count it
    write(old_fd[WRITE], total_size, sizeof(double));

    logSendPipe(*total_size);
}
