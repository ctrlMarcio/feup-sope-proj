#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "logger.h"

char FILENAME[] = "logfile.log";

void entry_log(int pid, char *action, char *info)
{
    int fd = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT);

    time_t rawtime;
    struct tm *timeinfo;

    char logentry[1024];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(logentry, "instant - %-10d - %-11s - %s\n", pid, action, info);

    write(fd, logentry, strlen(logentry));

    close(fd);
}