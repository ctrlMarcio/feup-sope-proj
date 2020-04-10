#ifndef LOGGER_H
#define LOGGER_H

#include "util.h"

#define CREATE "CREATE"
#define EXIT "EXIT"
#define RECV_SIGNAL "RECV_SIGNAL"
#define SEND_SIGNAL "SEND_SIGNAL"
#define RECV_PIPE "RECV_PIPE"
#define SEND_PIPE "SEND_PIPE"
#define ENTRY "ENTRY"

char FILENAME[256];

void entryLog(int pid, char *action, char *info);
void logCreate(struct flags *flags);
void logEntry(struct flags *flags, long size, char *name);
void logExit(int status);
void logRecvPipe(int size);
void logSendPipe(int size);
void logRecvSignal(int size);
void logSendSignal(int size, pid_t pid);

void setupLogger();

void closeLogger();

#endif