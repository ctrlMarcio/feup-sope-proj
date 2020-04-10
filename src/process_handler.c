#include "process_handler.h"
#include "logger.h"

#define y_ASCII 121
#define Y_ASCII 89
#define n_ASCII 110
#define N_ASCII 78

#define MAX_CHILDREN 2048

struct sigaction defaultsuspend;
pid_t children_pid[MAX_CHILDREN];
int qtchildren = 0;

void sig_int(int signo)
{
    logRecvSignal(signo);

    for (int i = 0; i < qtchildren; ++i)
        logSendSignal(SIGSTOP, children_pid[i]);

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
        for (int i = 0; i < qtchildren; ++i)
            logSendSignal(SIGTERM, children_pid[i]);
        killpg(childrenpg, SIGTERM);

        logSendSignal(SIGTERM, getpid());
        kill(getpid(), SIGTERM);
    }
    else
    {
        for (int i = 0; i < qtchildren; ++i)
            logSendSignal(SIGCONT, children_pid[i]);

        killpg(childrenpg, SIGCONT);
    }
}

void sig_term(int signo)
{
    logRecvSignal(signo);
    kill(getpid(), SIGKILL);
}

void sig_cont(int signo)
{
    logRecvSignal(signo);
}

void setupProcessHandlers()
{
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    struct sigaction sigterm_action;
    sigterm_action.sa_handler = sig_term;
    sigemptyset(&sigterm_action.sa_mask);
    sigterm_action.sa_flags = 0;
    sigaction(SIGTERM, &sigterm_action, NULL);

    struct sigaction sigcont_action;
    sigcont_action.sa_handler = sig_cont;
    sigemptyset(&sigcont_action.sa_mask);
    sigcont_action.sa_flags = 0;
    sigaction(SIGCONT, &sigcont_action, NULL);
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

    children_pid[qtchildren++] = pid;
}
