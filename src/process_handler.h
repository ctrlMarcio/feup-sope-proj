#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

#define PROCESS_ARRAY_SIZE 33554432

pid_t pids[PROCESS_ARRAY_SIZE];

int process_count;

void setup();
void setupChild();

void addProcess(pid_t pid);
void removeProcess(pid_t pid);
int getProcessIndex(pid_t pid);

void suspendProcesses();
void resumeProcesses();
void stopProcesses();