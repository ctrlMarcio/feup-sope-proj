#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include "logger.h"

#define MAX_HEADER_SIZE 512

char FILENAME[] = "logfile.log";

struct timespec START_TIME;

int LOGGER_FD;

void setupLogger()
{
    LOGGER_FD = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT, 0777);

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char tmp[MAX_HEADER_SIZE];
    sprintf(tmp, "Program executed at %2d:%2d, %d/%d/%d\n", local->tm_hour, local->tm_min, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    //write(LOGGER_FD, tmp, strlen(tmp));

    clock_gettime(CLOCK_MONOTONIC, &START_TIME);
}

void closeLogger()
{
    close(LOGGER_FD);
}

void entryLog(int pid, char *action, char *info)
{
    char logentry[1024];

    sprintf(logentry, "%.2f ms: PID %d. Action: %s. Additional info: %s\n", getElapsedTimeInMillis(START_TIME), pid, action, info);

    //write(LOGGER_FD, logentry, strlen(logentry));
}
