#include "ui.h"

void printFile(char *name, long size)
{
    printf("%-*ld%s\n", TABULATION, size, name);
}

void printDir(char *name, long size)
{
    printf("%-*ld%s\n", TABULATION, size, name);
}