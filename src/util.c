#include <stdio.h>
#include <string.h>
#include <time.h>

#include "util.h"

/**
 * Splits a string into an array
 * MODIFIES THE SOURCE STRING
 * E.g. after split("a,b,cd", arr, ","), arr = ["a","b","cd"];
 * 
 * @param string        string to split
 * @param dest          array to split into
 * @param delimitor     delimitor of the string
 * @return the size of the array
 */
int split(char *string, char *dest[], char *delimitor)
{
    char *word;
    int i = 0;

    word = strtok(string, delimitor);
    dest[i++] = word;

    while ((word = strtok(NULL, delimitor)))
        dest[i++] = word;

    return i;
}

double getElapsedTimeInMillis(struct timespec start_time)
{ 
    double elapsed = 0;

    struct timespec finish;

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start_time.tv_sec) / 1000.0;
    elapsed += (finish.tv_nsec - start_time.tv_nsec) / 1000000.0;

    return elapsed;
}