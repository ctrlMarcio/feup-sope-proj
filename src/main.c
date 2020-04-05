#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "util.h"
#include "simpledu.h"
#include "error.h"
#include "logger.h"

// to help clarification
#define TRUE 1
#define FALSE 0

#define LOG_FILENAME "LOG_FILENAME"

struct flags flags = {FALSE, ".", FALSE, FALSE, 1024, FALSE, FALSE, -1, 1};

void readArgs(int argc, char *argv[], int first_arg);

int main(int argc, char *argv[])
{
    readArgs(argc, argv, 1);

    if (!flags.count_links)
    {
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
        exit(ARGS_ERROR);
    }

    char *log_env;
    if ((log_env = getenv(LOG_FILENAME)))
        strncpy(FILENAME, log_env, strlen(log_env));

    char *line_args = (char*) malloc(sizeof(char) * 1024);

    for (int i = 1; i < argc; ++i)
    {
        strcat(line_args, argv[i]);
        if (i < argc - 1)
            strcat(line_args, " ");
    }

    strcpy(flags.line_args, line_args);

    setupLogger();

    fun(&flags);

    free(line_args);

    closeLogger();
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
    int arg = first_arg;

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "--count-links") || !strcmp(argv[arg], "-l"))
    {
        flags.count_links = TRUE;
        readArgs(argc, argv, ++arg);
        return;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-a") || !strcmp(argv[arg], "--all"))
    {
        flags.all = TRUE;
        readArgs(argc, argv, ++arg);
        return;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-b") || !strcmp(argv[arg], "--bytes"))
    {
        flags.bytes = TRUE;
        readArgs(argc, argv, ++arg);
        return;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-B"))
    {
        flags.block_size = atoi(argv[++arg]);
        if (flags.block_size == 0) // invalid value
        {
            printf("%s in an invalid block size.\n", argv[arg]);
            exit(ARGS_ERROR);
        }
        readArgs(argc, argv, ++arg);
        return;
    }

    char *tmp[10];
    char *tempstr = calloc(50, sizeof(char));
    strncpy(tempstr, argv[arg], strlen(argv[arg]));

    if (argc <= arg)
        return;
    if (split(tempstr, tmp, "=") == 2 && !strcmp(tmp[0], "--block-size"))
    {
        flags.block_size = atoi(tmp[1]);
        if (flags.block_size == 0) // invalid value
        {
            printf("%s in an invalid block size.\n", tmp[1]);
            exit(ARGS_ERROR);
        }
        readArgs(argc, argv, ++arg);
        return;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-L") || !strcmp(argv[arg], "--dereference"))
    {
        flags.dereference = TRUE;
        readArgs(argc, argv, ++arg);
        return;
    }

    if (argc <= arg)
        return;
    if (!strcmp(argv[arg], "-S") || !strcmp(argv[arg], "--separate-dirs"))
    {
        flags.separate_dirs = TRUE;
        readArgs(argc, argv, ++arg);
        return;
    }

    strncpy(tempstr, argv[arg], strlen(argv[arg]));
    if (argc <= arg)
        return;
    if (split(tempstr, tmp, "=") == 2 && !strcmp(tmp[0], "--max-depth"))
    {
        flags.max_depth = atoi(tmp[1]);
        if (flags.max_depth == 0) // invalid value
        {
            printf("%s in an invalid depth.\n", tmp[1]);
            exit(ARGS_ERROR);
        }
        readArgs(argc, argv, ++arg);
        return;
    }

    free(tempstr);

    if (argc <= arg)
        return;
    strcpy(flags.path, argv[arg]); // TODO: remove bars
    readArgs(argc, argv, ++arg);
    return;
}
