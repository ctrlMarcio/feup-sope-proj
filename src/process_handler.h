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

void setupProcessHandlers();
void setupParentHandlers();

#endif