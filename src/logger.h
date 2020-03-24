#ifndef LOGGER_H
#define LOGGER_H

#define CREATE "CREATE"
#define EXIT "EXIT"
#define RECV_SIGNAL "RECV_SIGNAL"
#define SEND_SIGNAL "SEND_SIGNAL"
#define RECV_PIPE "RECV_PIPE"
#define SEND_PIPE "SEND_PIPE"
#define ENTRY "ENTRY"

char FILENAME[256];

void entry_log(int pid, char *action, char *info);

#endif