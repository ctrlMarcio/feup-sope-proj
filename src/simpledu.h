/**
 * @file    simpledu.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Runs the main recursive function.
 * 
 */

#ifndef SIMPLEDU_H
#define SIMPLEDU_H

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "util.h"

/**
 * @brief   Runs the program itself, setting the first pipe and running the recursive function.
 * @warning It is expected that the flags (passed as argument), are already filled.
 * 
 * @param flags     the current flags of the program
 */
void run(struct flags *flags);

#endif