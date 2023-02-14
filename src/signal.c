#include "signal.h"
#include "prompt.h"
#include "process.h"

void ctrlc()
{
    write(STDOUT_FILENO, "\r\n", strlen("\r\n"));
    prompt();
    fflush(stdout);
}

void ctrlz()
{
    if (fg_pid != 0)
    {
        kill(fg_pid, SIGTSTP);
        fg_pid = 0;
    }
}

void child()
{
    int status;
    pid_t c_pid;
    while ((c_pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        int index = get_by_pid(&plist, c_pid);
        if (index == -1)
            continue;

        if (WIFEXITED(status))
        {
            if (plist.bg[index])
            {
                printf("\n%s with pid %d exited normally\n", plist.names[index], c_pid);
                prompt();
            }
            remove_processes(&plist, index);
        }
        else if (WIFSTOPPED(status))
        {
            printf("\n%s with pid %d suspended normally\n", plist.names[index], c_pid);
            plist.status[index] = 0;
            // prompt();
        }
        else if (WIFSIGNALED(status))
        {
            printf("\n%s with pid %d did not exit normally\n", plist.names[index], c_pid);
            remove_processes(&plist, index);
            prompt();
        }
    }
}