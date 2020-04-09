#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

pid_t childrenpg;

void setupProcessHandlers();
void setupParentHandlers();
void setChildrenGroup(pid_t);

#endif