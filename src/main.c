#include "headers.h"

char *HOME;
time_t start_time = 0;

int main()
{
    HOME = getcwd(NULL, 0);
    init_history(&history);
    init_history(&lastDir);
    init_processes(&plist);
    read_history();
    while (1)
    {
        char *inpBuf = (char *)malloc(sizeof(char) * 1024);
        prompt();
        signal(SIGINT, ctrlc);
        signal(SIGCHLD, child);
        signal(SIGTSTP, ctrlz);

        start_time = 0;
        if (fgets(inpBuf, 1024, stdin) == NULL)
        {
            exit(EXIT_SUCCESS);
        }

        add_history(&history, inpBuf);

        parse(inpBuf);
        free(inpBuf);
    }
}
