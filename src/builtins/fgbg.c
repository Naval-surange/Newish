#include "fgbg.h"

void fg(char **args, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (strcmp(args[i], "") == 0)
        {
            len = i;
            break;
        }
    }

    if (len == 1)
    {
        printf(RED "fg: Too few arguments\n" RESET);
        return;
    }
    if (len > 2)
    {
        printf(RED "fg: Too many arguments\n" RESET);
        return;
    }

    int pid_ind = atoi(args[1]);

    int index = pid_ind - 1;
    if (index < 0 || index >= plist.size)
    {
        printf(RED "fg: Invalid pid\n" RESET);
        return;
    }
    {
        if (kill(plist.pids[index], SIGCONT) == -1)
        {
            perror("fg:");
        }
        else
        {
            int status;
            plist.status[index] = 1;
            waitpid(plist.pids[index], &status, WUNTRACED);
        }
    }
}

void bg_fun(char **args, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (strcmp(args[i], "") == 0)
        {
            len = i;
            break;
        }
    }

    if (len == 1)
    {
        printf(RED "bg: Too few arguments\n" RESET);
        return;
    }

    if (len > 2)
    {
        printf(RED "bg: Too many arguments\n" RESET);
        return;
    }

    int pid_ind = atoi(args[1]);

    int index = pid_ind - 1;

    if (index < 0 || index >= plist.size)
    {
        printf(RED "bg: Invalid pid\n" RESET);
        return;
    }

    if (kill(plist.pids[index], SIGCONT) == -1)
    {
        perror("bg:");
    }
    else
    {
        plist.status[index] = 1;
    }

    return;
}