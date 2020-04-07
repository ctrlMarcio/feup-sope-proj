#ifndef UTIL_H
#define UTIL_H

#include <time.h>

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

/**
 * Splits a string into an array
 * MODIFIES THE SOURCE STRING
 * E.g. after split("a,b,cd", arr, ","), arr = ["a","b","cd"];
 * 
 * @param string            string to split
 * @param dest              array to split into
 * @param delimitor         delimitor of the string
 * @return the size of the array
 */
int split(char *string, char *dest[], char *delimitor);

/**
 * Removes any amount of a given character after a given string, copying the result to another one
 * 
 * @param c                 the character to remove
 * @param str               the original string
 * @param out               the string which will be changed with the final result
 * @return                  the size of the new string
 */
int remove_after(char c, const char *str, char *out);

/**
 * Gets the total elapsed time in milliseconds since a given time
 * 
 * @param start_time        the start time
 * @return                  smth like current_time - start_time in millisconds with precision up to nanoseconds
 */
double getElapsedTimeInMillis(struct timespec start_time);

#endif