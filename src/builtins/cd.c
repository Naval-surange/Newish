#include "../headers.h"
#include "../history.h"
#include "cd.h"

char *replaceTildaCD(const char *s)
{
    char *oldW = "~";
    char *newW = HOME;
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 100);

    i = 0;
    while (*s)
    {
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

void cd(char **args, int len)
{
    if (args[1] == NULL)
    {
        args[1] = (char *)malloc(sizeof(char) * 2);
        strcpy(args[1], "~");
    }
    if (len > 2)
    {
        printf(RED "cd: To many arguments\n" RESET);
        return;
    }

    char *cwd = getcwd(NULL, 0);
    add_history(&lastDir, cwd);

    char *path = replaceTildaCD(args[1]);
    if (strcmp(path, "~") == 0)
    {
        path = HOME;
    }
    else if (strcmp(path, "-") == 0)
    {
        path = lastDir.cmd[lastDir.size - 2];
    }

    if (chdir(path) != 0)
    {
        perror("cd");
    }
    return;
}
