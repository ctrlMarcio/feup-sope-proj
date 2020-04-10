/**
 * @file    logger.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Is responsible for logging the information on a logfile.
 * 
 */

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

/**
 * @brief   Setups the logfile, creating the file or appending if it already exists and writes a header.
 * @warning Use before any logger.
 * 
 */
void setupLogger();

/**
 * @brief   Closes the logfile.
 * @warning Use only at the end of the program, no logs will be processed after.
 * 
 */
void closeLogger();

/**
 * @brief   Logs the creation of a process.
 * 
 * @param flags     the current flags of the program
 */
void logCreate(struct flags *flags);

/**
 * @brief   Logs the analysing of a file or directory.
 * 
 * @param flags     the current flags of the program
 * @param size      the number of bytes of the entry
 * @param name      tha path of the entry
 */
void logEntry(struct flags *flags, long size, char *name);

/**
 * @brief   Logs the exit of a process.
 * 
 * @param status    the exit status of the process
 */
void logExit(int status);

/**
 * @brief   Logs the receiving of a pipe.
 * @details Pipes only share sizes.
 * 
 * @param size      the size received
 */
void logRecvPipe(int size);

/**
 * @brief   Logs the sending of a pipe.
 * @details Pipes only share sizes.
 * 
 * @param size      the size sent
 */
void logSendPipe(int size);

/**
 * @brief   Logs the receiving of a signal.
 * 
 * @param signo     the signal received
 */
void logRecvSignal(int signo);

/**
 * @brief   Logs the sending of a signal.
 * 
 * @param signo     the signal sent
 * @param pid       the destination process id
 */
void logSendSignal(int signo, pid_t pid);

#endif