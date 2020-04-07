#ifndef SIMPLEDU_H
#define SIMPLEDU_H

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "util.h"

/**
 * Creates a pipe and runs
 * 
 * @param flags
 */
void run(struct flags *flags);

#endif