#include "headers.h"
#include "builtins/cd.h"
#include "builtins/ls.h"
#include "builtins/echo.h"
#include "builtins/pinfo.h"
#include "builtins/discover.h"
#include "builtins/jobs.h"
#include "builtins/sig.h"
#include "builtins/fgbg.h"
#include "history.h"
#include "execute.h"
#include "process.h"

void execute(char **args, int len, int bg)
{

    if (len < 1 || args[0] == NULL || strcmp(args[0], "") == 0 || strcmp(args[0], "\0") == 0)
        return;

    if (strcmp(args[0], "cd") == 0)
        cd(args, len);
    else if (strcmp(args[0], "ls") == 0)
        ls(args, len);
    else if (strcmp(args[0], "echo") == 0)
        echo(args, len);
    else if (strcmp(args[0], "history") == 0)
        print_history(&history, args, len);
    else if (strcmp(args[0], "pinfo") == 0)
        pinfo(args, len);
    else if (strcmp(args[0], "discover") == 0)
        discover(args, len);
    else if (strcmp(args[0], "jobs") == 0)
        jobs(args, len);
    else if (strcmp(args[0], "sig") == 0)
        sig(args, len);
    else if (strcmp(args[0], "fg") == 0)
        fg(args, len);
    else if (strcmp(args[0], "bg") == 0)
        bg_fun(args, len);
    else if (strcmp(args[0], "pwd") == 0)
    {
        if (len > 1)
        {
            printf(RED "pwd : To many arguments\n" RESET);
        }
        else
        {
            char *cwd = getcwd(NULL, 0);
            printf("%s\n", cwd);
        }
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        write_history(&history);
        exit(0);
    }
    else
    {
        pid_t pid;
        int status;
        pid = fork();
        if (pid == 0)
        {
            if (bg)
            {
                setpgid(0, 0);
            }

            char **newArgs = (char **)malloc(sizeof(char *) * (len + 1));
            int newLen = 0;
            for (int i = 0; i < len; i++)
            {
                if (strcmp(args[i], "") != 0)
                {
                    newArgs[i] = args[i];
                    newLen++;
                }
            }
            newArgs[newLen] = NULL;
            int exec_status = execvp(newArgs[0], newArgs);
            if (exec_status == -1)
            {
                perror("Error");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            add_processes(&plist, args[0], pid, bg);
            fg_pid = pid;
            if (!bg)
            {
                time_t st_time = time(NULL);

                pause();

                time_t end_time = time(NULL);
                start_time += end_time - st_time;
            }
            else
            {
                printf("%s %d\n", args[0], pid);
            }
        }
    }
}