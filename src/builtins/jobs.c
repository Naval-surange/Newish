#include "../headers.h"
#include "jobs.h"
#include "../process.h"

void jobs(char **args, int len)
{

    char **args_wo_flag = (char **)malloc(sizeof(char *) * len);
    int no_args_wo_flag = 0;

    int run_f = 0, sleep_f = 0;

    for (int i = 0; i < len; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] != 'r' && args[i][j] != 's')
                {
                    printf(RED "discover: invalid option -- '%c'\n" RESET, args[i][j]);
                    return;
                }
                if ((args[i][j] == 'r' && run_f) || (args[i][j] == 's' && sleep_f))
                {
                    printf(RED "discover: option '%c' is already set\n" RESET, args[i][j]);
                    return;
                }
                if (args[i][j] == 'r')
                {
                    run_f = 1;
                }
                if (args[i][j] == 's')
                {
                    sleep_f = 1;
                }
            }
        }
        else
        {
            args_wo_flag[no_args_wo_flag] = args[i];
            no_args_wo_flag++;
        }
    }
    if (!sleep_f && !run_f)
    {
        sleep_f = 1;
        run_f = 1;
    }

    for (int i = 0; i < plist.size; i++)
    {
        if (run_f && plist.bg[i])
        {
            printf("[%d] Running %s [%d]\n", i + 1, plist.names[i], plist.pids[i]);
        }
    }
}