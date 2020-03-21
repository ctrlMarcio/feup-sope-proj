#include <stdio.h>
#include <string.h>

#include "util.h"

/**
 * Splits a string into an array
 * E.g. after split("a,b,cd", arr, ","), arr = ["a","b","cd"];
 * 
 * @param string        string to split
 * @param dest          array to split into
 * @param delimitor     delimitor of the string
 * @return the size of the array
 */
int split(char *string, char *dest[], char *delimitor) {
    char *word;
    int i = 0;

    word = strtok(string, delimitor);
    dest[i++] = word;

    while ((word = strtok(NULL, delimitor)))
        dest[i++] = word;

    return i;
}