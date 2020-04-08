#include "ui.h"

#include <string.h>
#include <unistd.h>

void printFile(char *name, struct flags *flags, long size)
{
    if (!flags->all)
        return;

    if (!flags->bytes)
        size /= flags->block_size;

    char line[1024];

    if (flags->linked)
    {
        sprintf(line, "%s/%s", flags->link_path, name);
    }
    else
    {
        sprintf(line, "%s/%s", flags->path, name);
    }

    char tmp[200];
    sprintf(tmp, "%-ld\t%s\n", size, line);
    write(STDOUT_FILENO, tmp, strlen(tmp));
}

void printDir(char *name, struct flags *flags, long size)
{
    if (!flags->bytes)
        size /= flags->block_size;

    char line[1024];

    if (flags->linked)
    {
        sprintf(line, "%s", flags->link_path);
    }
    else
    {
        sprintf(line, "%s", flags->path);
    }

    char tmp[200];
    sprintf(tmp, "%-ld\t%s\n", size, line);
    write(STDOUT_FILENO, tmp, strlen(tmp));
}

void printLink(char *name, struct flags *flags, long size)
{
    if (!flags->bytes)
        size /= flags->block_size;

    char tmp[200];
    sprintf(tmp, "%-ld\t%s\n", size, name);
    write(STDOUT_FILENO, tmp, strlen(tmp));
}