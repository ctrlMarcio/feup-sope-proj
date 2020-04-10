/**
 * @file    ui.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Responsible for printing to the STDOUT the results of the program.
 * 
 */

#ifndef UI_H
#define UI_H

#include <stdio.h>

#include "util.h"

/**
 * @brief   Prints a regular file according to the flags.
 * 
 * @param name      the name of the file
 * @param flags     the executable's flags
 * @param size      the size of the file, in bytes
 */
void printFile(char *name, struct flags *flags, long size);

/**
 * @brief   Prints a directory according to the flags.
 * 
 * @param name      the name of the directory
 * @param flags     the executable's flags
 * @param size      the total size of the directory, in bytes
 */
void printDir(char *name, struct flags *flags, long size);

/**
 * @breif   Prints a directory according to the flags.
 * 
 * @param name      the name of the directory
 * @param flags     the executable's flags
 * @param size      the total size of the directory, in bytes
 */
void printLink(char *name, struct flags *flags, long size);

#endif