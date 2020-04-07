#include "ui.h"

#include <string.h>
#include <unistd.h>

void printFile(char *name, struct flags *flags, long size)
{
    if (!flags->all)
        return;

    if (!flags->bytes)
        size /= flags->block_size;

    char tmp[200];
    sprintf(tmp, "%-ld\t%s\n", size, name);
    write(STDOUT_FILENO, tmp, strlen(tmp));
}

void printDir(char *name, struct flags *flags, long size)
{
    if (!flags->bytes)
        size /= flags->block_size;

    char tmp[200];
    sprintf(tmp, "%-ld\t%s\n", size, name);
    write(STDOUT_FILENO, tmp, strlen(tmp));
}