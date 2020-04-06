#include "ui.h"

void printFile(char *name, struct flags *flags, long size)
{
    if (!flags->bytes) 
        size /= flags->block_size;

    printf("%-*ld%s\n", TABULATION, size, name);
}

void printDir(char *name, struct flags *flags, long size)
{
    if (!flags->bytes) 
        size /= flags->block_size;
    printf("%-*ld%s\n", TABULATION, size, name);
}