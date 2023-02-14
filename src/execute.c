#include "headers.h"
#include "builtins/cd.h"
#include "builtins/ls.h"
#include "builtins/echo.h"
#include "builtins/pinfo.h"
#include "builtins/discover.h"
#include "builtins/jobs.h"
#include "history.h"
#include "execute.h"
#include "process.h"

void execute(char **args, int len, int bg)
{

    if (len < 1 || args[0] == NULL || strcmp(args[0], "") == 0 || strcmp(args[0], "\0") == 0)
        return;

    // start_time = time(NULL);

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

            int exec_status = execvp(args[0], args);
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
            if (!bg)
            {
                time_t st_time = time(NULL);

                waitpid(pid, &status, WUNTRACED);
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