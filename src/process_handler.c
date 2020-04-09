#include "process_handler.h"

#define y_ASCII 121
#define Y_ASCII 89
#define n_ASCII 110
#define N_ASCII 78

pid_t pids[PROCESS_ARRAY_SIZE] = {0};

int process_count = 0;

void sig_int(int signo)
{
    printf("\n\nTEST0\n\n");
    suspendProcesses();

    char buffer;
    write(STDOUT_FILENO, "Do you really want to stop the program? (Y/N) ", 46);
    read(STDIN_FILENO, &buffer, 1);

    while (buffer != y_ASCII && buffer != Y_ASCII && buffer != n_ASCII && buffer != N_ASCII)
    {
        write(STDOUT_FILENO, "Invalid option. Do you really want to stop the program? (Y/N) ", 62);
        read(STDIN_FILENO, &buffer, 1);
    }

    if (buffer == y_ASCII || buffer == Y_ASCII)
    {
        stopProcesses();
    }
    else
    {
        resumeProcesses();
    }
}

void sig_tstp(int signo)
{
    printf("\n\nTEST1\n\n");
    suspendProcesses();
    printf("\n\nTEST2\n\n");
    sigset_t sigmask;
    sigfillset(&sigmask);
    sigdelset(&sigmask, SIGCONT);

    sigsuspend(&sigmask);
    printf("\n\nTEST3\n\n");
}

void sig_cont(int signo)
{
    resumeProcesses();
}

void setup()
{
    struct sigaction action;
    action.sa_handler = sig_int;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
}

void setupChild()
{
    memset(pids, 0, sizeof pids);

    process_count = 0;

    struct sigaction suspend;
    suspend.sa_handler = sig_tstp;
    sigemptyset(&suspend.sa_mask);
    suspend.sa_flags = 0;
    sigaction(SIGTSTP, &suspend, NULL);

    struct sigaction cont;
    cont.sa_handler = sig_cont;
    sigemptyset(&cont.sa_mask);
    cont.sa_flags = 0;
    sigaction(SIGCONT, &cont, NULL);
}

void addProcess(pid_t pid)
{
    if (getProcessIndex(pid) != -1)
        return;

    pids[process_count++] = pid;
}

void removeProcess(pid_t pid)
{
    int index = getProcessIndex(pid);

    if (index == -1)
        return;

    for (int i = index; i < process_count - 1; ++i)
        pids[index] = pids[index + 1];

    process_count--;
}

int getProcessIndex(pid_t pid)
{
    for (int i = 0; i < process_count; ++i)
        if (pid == pids[i])
            return i;

    return -1;
}

void suspendProcesses()
{
    for (int i = 0; i < process_count; ++i)
    {
        printf("\n\nTEST: %d\n\n", pids[i]);
        kill(pids[i], SIGTSTP);
    }
}

void resumeProcesses()
{
    for (int i = 0; i < process_count; ++i)
        kill(pids[i], SIGCONT);
}

void stopProcesses()
{
    for (int i = 0; i < process_count; ++i)
        kill(pids[i], SIGKILL);
}