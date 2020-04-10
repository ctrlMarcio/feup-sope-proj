/**
 * @file    util.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Holds important extra structs and functions. 
 * 
 */

#ifndef UTIL_H
#define UTIL_H

#include <time.h>

/**
 * @brief   Holds information on all aspects of the program.
 * 
 */
struct flags
{
    int count_links;       // bool - counts the same file multiple times
    char path[512];        // current directory "." is the default directory of "du"
    int all;               // bool - include files
    int bytes;             // bool - show in bytes instead of blocks
    int block_size;        // size of the blocks (in bytes), for visualization effects
    int dereference;       // bool - follow symbolic links
    int separate_dirs;     // bool - do not include the size of a subdirectory in a directory
    int max_depth;         // limit the depth of the directories (-1 for infinite)
    int current_depth;     // current depth of the search
    char line_args[1024];  // the arguments
    int linked;            // bool - content is in a link or not
    char link_path[512];   // the path of the link
    int inodes_read[4096]; // the inodes of the link read
    int inodes_read_size;
};

/**
 * @brief   Splits a string into an array.
 * @warning Modifies the source string.
 * @details E.g. after split("a,b,cd", arr, ","), arr = ["a","b","cd"]
 * 
 * @param string            the string to split
 * @param dest              the array to split into
 * @param delimitor         the delimitor of the string
 * @return  the size of the array
 */
int split(char *string, char *dest[], char *delimitor);

/**
 * @brief   Removes any amount of a given character after a given string, copying the result to another one.
 * 
 * @param c                 the character to remove
 * @param str               the original string
 * @param out               the string which will be changed with the final result
 * @return  the size of the new string
 */
int remove_after(char c, const char *str, char *out);

/**
 * @brief   Gets the total elapsed time in milliseconds since a given time.
 * 
 * @param start_time        the start time
 * @return  the difference in milliseconds (up to nanoseconds) between the current time and the start time
 */
double getElapsedTimeInMillis(struct timespec start_time);

/**
 * @brief   Verifies whether an element is contained in a vector or not.
 * 
 * @param element           the element to verify
 * @param vec               the vector
 * @param size              the size of the vector
 * @return  true if the element is contained in the vector, false otherwise
 */
int contains(int element, int *vec, int size);

#endif