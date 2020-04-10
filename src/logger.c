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

char FILENAME[] = "../logfile.log";

struct timespec START_TIME;

int LOGGER_FD;

void logEntry(struct flags *flags, long size, char *name);

void setupLogger()
{
    LOGGER_FD = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT, 0777);

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char tmp[MAX_HEADER_SIZE];
    sprintf(tmp, "Program executed at %02d:%02d, %02d/%02d/%04d\n", local->tm_hour, local->tm_min, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    write(LOGGER_FD, tmp, strlen(tmp));

    clock_gettime(CLOCK_MONOTONIC, &START_TIME);
}

void closeLogger()
{
    close(LOGGER_FD);
}

void entryLog(int pid, char *action, char *info)
{
    char logentry[1024];

    sprintf(logentry, "%.2f ms: PID %08d. Action: %s. Additional info: %s\n", getElapsedTimeInMillis(START_TIME), pid, action, info);

    write(LOGGER_FD, logentry, strlen(logentry));
}

void logEntry(struct flags *flags, long size, char *name)
{
    if (!flags->bytes)
        size /= flags->block_size;
    char log_entry[128];
    sprintf(log_entry, "%-ld\t%s", size, name);

    entryLog(getpid(), ENTRY, log_entry);
}

void logCreate(struct flags *flags)
{
    entryLog(getpid(), CREATE, flags->line_args);
}

void logExit(int status)
{
    char line[32];
    sprintf(line, "termination code %d", WEXITSTATUS(status));
    entryLog(getpid(), EXIT, line);
}

void logRecvPipe(int size)
{
    char log_entry[32];
    sprintf(log_entry, "%d", size);

    entryLog(getpid(), RECV_PIPE, log_entry);
}

void logSendPipe(int size)
{
    char log_entry[32];
    sprintf(log_entry, "%d", size);
    entryLog(getpid(), SEND_PIPE, log_entry);
}

void logRecvSignal(int signal)
{
    char log_entry[32];
    sprintf(log_entry, "%d", signal);
    entryLog(getpid(), RECV_SIGNAL, log_entry);
}

void logSendSignal(int signal, pid_t pid)
{
    char log_entry[64];
    sprintf(log_entry, "Signal: %d, Destination PID: %d", signal, pid);
    entryLog(getpid(), SEND_SIGNAL, log_entry);
}