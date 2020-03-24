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

#define READ 0
#define WRITE 1
#define MAX_SIZE 2048
#define MAX_DIR_SIZE 4096
#define TABULATION 8

void fun(struct flags *flags)
{
    int fd[2];
    pipe(fd);
    dup2(STDOUT_FILENO, fd[WRITE]);
    simpledu(flags, fd);
}

void simpledu(struct flags *flags, int *old_fd)
{
    struct dirent *dirent;
    struct stat stat_entry;

    char dirBuffer[MAX_DIR_SIZE] = "";

    DIR *dir;

    if ((dir = opendir(flags->path)) == NULL)
    {
        // TODO: error
        exit(DIR_ERROR);
    }

    while ((dirent = readdir(dir)) != NULL)
    {
        entry_log(getpid(), CREATE, "TO-BE-DONE-EVENTUALLY");

        char file[MAX_SIZE];
        sprintf(file, "%s/%s", flags->path, dirent->d_name);
        lstat(file, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode))
        {
            if (!strcmp(dirent->d_name, ".."))
                continue;
            if (flags->current_depth == 1 && !strcmp(dirent->d_name, "."))
            {
                char string[MAX_SIZE];
                sprintf(string, "%-*ld%s\n", TABULATION, stat_entry.st_size, dirent->d_name); // size in bytes
                strcat(dirBuffer, string);
            }
            if (!strcmp(dirent->d_name, "."))
                continue;

            char string[MAX_SIZE];
            sprintf(string, "%-*ld%s\n", TABULATION, stat_entry.st_size, file); // size in bytes
            strcat(dirBuffer, string);

            if (flags->current_depth >= flags->max_depth && flags->max_depth > 0)
                continue;

            pid_t pid = treatDir(old_fd, flags, dirent);
            if (pid == 0)
                return;
        }
        else if (S_ISREG(stat_entry.st_mode))
        {
            char string[MAX_SIZE];
            sprintf(string, "%-*ld%s\n", TABULATION, stat_entry.st_size, file); // size in bytes

            strcat(dirBuffer, string);
        }
        else if (S_ISLNK(stat_entry.st_mode))
        {
            // TODO:
            printf("Link\n");
        }
    }

    if (closedir(dir) == -1)
    {
        // TODO: error
        exit(DIR_ERROR);
    }

    write(old_fd[WRITE], dirBuffer, strlen(dirBuffer));

    while (wait(NULL) != -1)
        ;
}

int treatDir(int *old_fd, struct flags *flags, struct dirent *dirent)
{
    int fd[2];

    if (pipe(fd) < 0)
    { /* TODO: error*/
        exit(PIPE_ERROR);
    }

    dup2(old_fd[WRITE], fd[WRITE]);

    pid_t pid = fork();
    if (pid == -1)
    { /* TODO: error */
        exit(FORK_ERROR);
    }
    else if (pid == 0)
    { // child
        close(fd[READ]);
        struct flags tmp_flags = *flags;
        strcat(tmp_flags.path, "/");
        strcat(tmp_flags.path, dirent->d_name);

        tmp_flags.current_depth++;

        simpledu(&tmp_flags, fd);
    }
    // parent just continues
    return pid;
}
