#include "signal.h"
#include "prompt.h"
#include "process.h"

// void setup_sighandler(int SIG, void (*handler)(int, siginfo_t*, void*)){
// 	struct sigaction sa;
// 	memset(&sa, 0, sizeof(struct sigaction));
// 	sa.sa_sigaction = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags =  SA_RESTART | SA_SIGINFO;
// 	check_fatal_perror("Signal handler", sigaction(SIG, &sa, NULL), -1);
// }

// void signalHandler(int SIGNAL, void *handel(int, siginfo_t *, void *))
// {
//     struct sigaction new_action, old_action;

//     memset(&sa, 0, sizeof(struct sigaction));
//     sa.sa_sigaction = handel;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = SA_RESTART | SA_SIGINFO;
// }

void ctrlc()
{
    write(STDOUT_FILENO, "\r\n", strlen("\r\n"));
    prompt();
    fflush(stdout);
}

void ctrlz()
{
    write(STDOUT_FILENO, "\r\n", strlen("\r\n"));
    prompt();
    fflush(stdout);
}
void child()
{
    int status;
    pid_t c_pid;
    setpgid(0, 0);
    while ((c_pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        int index = get_by_pid(&plist, c_pid);
        // if (index == -1)
        //     continue;

        if (WIFEXITED(status))
        {
            printf("\n%s with pid %d exited normally\n", plist.names[index], c_pid);
            remove_processes(&plist, index);
        }
        else if (WIFSTOPPED(status))
        {
            printf("\n%s with pid %d suspended normally\n", plist.names[index], c_pid);
        }
        else if (WIFSIGNALED(status))
        {
            printf("\n%s with pid %d did not exit normally\n", plist.names[index], c_pid);
            remove_processes(&plist, index);
        }

        if (plist.bg[index] == 1)
        {
            prompt();
        }
    }
}