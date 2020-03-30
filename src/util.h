#ifndef UTIL_H
#define UTIL_H

struct flags
{
    int count_links;        // bool - counts the same file multiple times
    char path[512];         // current directory "." is the default directory of "du"
    int all;                // bool - include files
    int bytes;              // bool - show in bytes instead of blocks
    int block_size;         // size of the blocks (in bytes), for visualization effects
    int dereference;        // bool - follow symbolic links
    int separate_dirs;      // bool - do not include the size of a subdirectory in a directory
    int max_depth;          // limit the depth of the directories (-1 for infinite)
    int current_depth;      // current depth of the search
    char line_args[1024];   // the arguments
};

int split(char *string, char *dest[], char *delimitor);

double getElapsedTimeInMillis();

#endif