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

void fun(struct flags *flags)
{
    int fd[2];
    pipe(fd);
    simpledu(flags, fd);
}

void simpledu(struct flags *flags, int *old_fd)
{
    struct dirent *dirent;
    struct stat stat_entry;

    char dir_buffer[MAX_DIR_SIZE] = "";

    DIR *dir;
    int total_size = 0;

    if ((dir = opendir(flags->path)) == NULL)
    {
        perror(flags->path);
        exit(DIR_ERROR);
    }

    while ((dirent = readdir(dir)) != NULL)
    {
        char file[MAX_SIZE];
        sprintf(file, "%s/%s", flags->path, dirent->d_name);
        lstat(file, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode))
        {

            if (!strcmp(dirent->d_name, ".."))
                continue;

            if (!strcmp(dirent->d_name, ".")) {
                total_size += stat_entry.st_size;
                continue;
            }

            if (flags->separate_dirs && flags->current_depth > flags->max_depth && flags->max_depth > 0) {
                continue;
            }
    
            int dir_size = treatDir(old_fd, flags, dirent, &stat_entry);
            if (dir_size == -1)
                exit(0);
            total_size += dir_size;
        }
        else if (S_ISREG(stat_entry.st_mode))
        {
            total_size += stat_entry.st_size;
            if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                printFile(file, flags, stat_entry.st_size);
        }
        else if (S_ISLNK(stat_entry.st_mode))
        {
            // TODO change later
            if (!(flags->current_depth > flags->max_depth && flags->max_depth > 0))
                printFile(file, flags, stat_entry.st_size);
        }
    }

    if (closedir(dir) == -1)
    {
        perror(flags->path);
        exit(DIR_ERROR);
    }

    entryLog(getpid(), RECV_PIPE, dir_buffer);

    int status;
    int pid;

    while ((pid = wait(&status)) > 0)
    {
        char line[32];
        sprintf(line, "termination code %d", WEXITSTATUS(status));
        entryLog(pid, EXIT, line);
    }

    if (!(flags->current_depth - 1 > flags->max_depth && flags->max_depth > 0))
        printDir(flags->path, flags, total_size);
        
    write(old_fd[WRITE], &total_size, sizeof(int));
}

int treatDir(int *old_fd, struct flags *flags, struct dirent *dirent, struct stat *stat_entry)
{
    int total_size = 0;

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Creating pipe on new directory");
        exit(PIPE_ERROR);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Forking");
        exit(FORK_ERROR);
    }
    else if (pid == 0)
    { // child
        struct flags tmp_flags = *flags;
        strcat(tmp_flags.path, "/");
        strcat(tmp_flags.path, dirent->d_name);

        tmp_flags.current_depth++;

        simpledu(&tmp_flags, fd);
        return -1;
    }
    else
    {
        entryLog(pid, CREATE, flags->line_args);
        int tmp;
        while (wait(NULL) > 0) {
            read(fd[READ], &tmp, sizeof(int));
            total_size += tmp;
        }
    }
    return total_size;
}
