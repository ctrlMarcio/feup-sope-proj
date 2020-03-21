#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "util.h"

// to help clarification
#define TRUE 1
#define FALSE 0

int count_links = FALSE;   // bool - counts the same file multiple times
char *path = ".";          // current directory "." is the default directory of "du"
int all = FALSE;           // bool - include files
int bytes = FALSE;         // bool - show in bytes instead of blocks
int block_size = 1024;     // size of the blocks (in bytes), for visualization effects
int dereference = FALSE;   // bool - follow symbolic links
int separate_dirs = FALSE; // bool - do not include the size of a subdirectory in a directory
int max_depth = -1;        // limit the depth of the directories (-1 for infinite)

void readArgs(int argc, char *argv[], int first_arg);

int main(int argc, char *argv[], char *argenv[])
{
    if (argc < 2 || (strcmp(argv[1], "--count-links") && strcmp(argv[1], "-l")))
    {
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
        exit(1);
    }

    readArgs(argc, argv, 2); // two bc -l is required always
}

/**
 * Reads the args from the command line
 * 
 * @param argc          the total amount of args
 * @param argv          the arguments
 * @param first_arg     the position of the first argument to read
 */
void readArgs(int argc, char *argv[], int first_arg)
{
    if (!strcmp(argv[1], "--count-links") || !strcmp(argv[1], "-l"))
        count_links = TRUE;

    int arg = first_arg;

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-a") || !strcmp(argv[arg], "--all"))
    {
        all = TRUE;
        arg++;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-b") || !strcmp(argv[arg], "--bytes"))
    {
        bytes = TRUE;
        arg++;
    }

    char *tmp[10];

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-B"))
    {
        block_size = atoi(argv[++arg]);
        if (block_size == 0) // invalid value
        {
            printf("%s in an invalid block size.\n", argv[arg]);
            exit(2);
        }
        arg++;
    } else if (split(argv[arg], tmp, "=") == 2 && !strcmp(tmp[0], "--block-size"))
    {
        block_size = atoi(tmp[1]);
        if (block_size == 0) // invalid value
        {
            printf("%s in an invalid block size.\n", tmp[1]);
            exit(2);
        }
        arg++;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-L") || !strcmp(argv[arg], "--dereference"))
    {
        dereference = TRUE;
        arg++;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-S") || !strcmp(argv[arg], "--separate-dirs"))
    {
        separate_dirs = TRUE;
        arg++;
    }

    if (argc <= arg)
        return;
    if (split(argv[arg], tmp, "=") == 2 && !strcmp(tmp[0], "--max-depth"))
    {
        max_depth = atoi(tmp[1]);
        if (max_depth == 0) // invalid value
        {
            printf("%s in an invalid depth.\n", tmp[1]);
            exit(3);
        }
        arg++;
    }

    if (argc <= arg)
        return;
    if (arg != 2)
    {
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
        exit(1);
    }

    path = argv[arg];
    readArgs(argc, argv, 3);
}