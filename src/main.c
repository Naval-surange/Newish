#include "headers.h"
#include "io.h"

char *HOME;
time_t start_time = 0;
pid_t shell_pid;
pid_t fg_pid = 0;
int main()
{
    HOME = getcwd(NULL, 0);
    init_history(&history);
    init_history(&lastDir);
    init_processes(&plist);
    read_history();
    shell_pid = getpid();
    signal(SIGINT, ctrlc);
    signal(SIGTSTP, ctrlz);
    signal(SIGCHLD, child);
    char *inpBuf = malloc(sizeof(char) * 1024);
    while (1)
    {
        prompt();
        start_time = 0;
        handleIO(inpBuf);
        add_history(&history, inpBuf);
        parse(inpBuf);
    }
    free(inpBuf);
}
