#include <stdio.h>
#include <string.h>
#include <time.h>

#include "util.h"

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

// inspired by
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
// credit Adam Rosenfield
int remove_after(char c, const char *str, char *out)
{
    int len = strlen(str);
    if (len == 0)
        return 0;

    const char *end;
    size_t out_size;

    // Trim trailing character
    end = str + strlen(str) - 1;
    while (end > str && *end == c)
        end--;
    end++;

    out_size = end - str;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
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