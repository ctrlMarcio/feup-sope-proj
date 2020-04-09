#include "process_handler.h"

#define y_ASCII 121
#define Y_ASCII 89
#define n_ASCII 110
#define N_ASCII 78

struct sigaction defaultsuspend;

void sig_int(int signo)
{
    killpg(childrenpg, SIGSTOP);
    char buffer[2];
    write(STDOUT_FILENO, "Do you really want to stop the program? (Y/N) ", 46);
    read(STDIN_FILENO, &buffer, 2);

    while (*buffer != y_ASCII && *buffer != Y_ASCII && *buffer != n_ASCII && *buffer != N_ASCII)
    {
        write(STDOUT_FILENO, "Invalid option. Do you really want to stop the program? (Y/N) ", 62);
        read(STDIN_FILENO, &buffer, 2);
    }

    if (*buffer == y_ASCII || *buffer == Y_ASCII)
    {
        killpg(childrenpg, SIGTERM);
        killpg(0, SIGTERM);
    }
    else
    {
        killpg(childrenpg, SIGCONT);
    }
}

void setupProcessHandlers()
{
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
}

void setupParentHandlers()
{
    struct sigaction action;
    action.sa_handler = sig_int;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
}

void setChildrenGroup(pid_t pid)
{
    if (!childrenpg)
    {
        childrenpg = pid;
        setpgid(pid, pid);
    }
    else
    {
        setpgid(pid, childrenpg);
    }
}
