/**
 * @file    process_handler.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Handles the signals of the processes.
 * 
 */

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

/**
 * @brief   Process ID of the children process group.
 * @details With the exception of the very first process (the parent), all processes will be assigned the same process group,
 * childrenpg is the ID of this process group.
 * @details The ID of the process group is the same as the ID of the first child.
 * 
 */
pid_t childrenpg;

/**
 * @brief   Sets up the handlers for a child process.
 * @details Ignores SIGINT (only the parent should receive it).
 * @details Logs and exits upon receiving SIGTERM.
 * @details Logs upon receiving SIGCONT.
 * 
 */
void setupProcessHandlers();

/**
 * @brief   Sets up the handlers for the parent process.
 * @details Upon receiving a SIGINT, sends a SIGSTOP to all its children. Then, asks the user if he really wants to terminate the program, 
 * sending SIGTERM to all its children if the user agrees, or SIGCONT if it wants to continue the program.
 * 
 */
void setupParentHandlers();

/**
 * @brief   Sets a process to the children group.
 * @details If the children group is empty, assigns its ID to the ID of this child. If not, only sets this child's pg ID accordingly.
 * 
 * @param pid       the ID of the process to set to the children group
 */
void setChildrenGroup(pid_t pid);

#endif