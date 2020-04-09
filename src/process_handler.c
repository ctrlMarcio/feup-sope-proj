#include "process_handler.h"

#define y_ASCII 121
#define Y_ASCII 89
#define n_ASCII 110
#define N_ASCII 78

void sig_int(int signo)
{
    kill(0, SIGTSTP);
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
        kill(0, SIGKILL);
    }
    else
    {
        kill(0, SIGCONT);
    }
}

void setupProcessHandlers() {
    struct sigaction suspend;
    suspend.sa_handler = SIG_IGN;
    sigemptyset(&suspend.sa_mask);
    suspend.sa_flags = 0;
    sigaction(SIGINT, &suspend, NULL);
}

void setupParentHandlers()
{
    struct sigaction action;
    action.sa_handler = sig_int;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    struct sigaction suspend;
    suspend.sa_handler = SIG_IGN;
    sigemptyset(&suspend.sa_mask);
    suspend.sa_flags = 0;
    sigaction(SIGTSTP, &suspend, NULL);
}
