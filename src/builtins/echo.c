#include "../headers.h"
#include "echo.h"

void echo(char **args, int len)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "echo: No arguments given\n");
        return;
    }
    else
    {
        for (int i = 1; i < len; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    return;
}