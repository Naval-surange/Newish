#include "../headers.h"

void sig(char **args, int len)
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
        printf(RED "sig: Too few arguments\n" RESET);
        return;
    }
    if (len > 3)
    {
        printf(RED "sig: Too many arguments\n" RESET);
        return;
    }

    int pid_ind = atoi(args[1]);
    int sig = atoi(args[2]);


    int index = pid_ind-1;
    if (index < 0 || index >= plist.size)
    {
        printf(RED "sig: Invalid pid\n" RESET);
        printf("USAGE: sig <pid> <signal>\n");
        return;
    }
    {
        if (kill(plist.pids[index], sig) == -1)
        {
            perror("Sig:");
        }
        else
        {
            printf("Sent signal %d to process %d\n", sig, plist.pids[index]);
        }
    }
}
