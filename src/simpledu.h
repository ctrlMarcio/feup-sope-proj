#ifndef SIMPLEDU_H
#define SIMPLEDU_H

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "util.h"

void fun(struct flags *flags);
void simpledu(struct flags *flags, int *fd);
int treatDir(int *old_fd, struct flags *flags, struct dirent *dirent);

#endif