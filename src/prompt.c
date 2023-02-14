#include "headers.h"

char *replaceWord(const char *s)
{
    char *oldW = HOME;
    char *newW = "~";
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
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

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

void prompt()
{
    fg_pid = 0;
    struct utsname my_uname;
    if (uname(&my_uname) < 0)
    {
        perror("uname");
        exit(1);
    }
    char *cwd = getcwd(NULL, 0);
    cwd = replaceWord(cwd);
    char *uname = getlogin();
    char *nodename = my_uname.nodename;
    char *buf = (char *)malloc(sizeof(char) * 256);

    if (start_time >= 1)
    {
        printf(GRN "%s@%s" RESET ":" BLU "%s" RESET " |%lds⏳|$ ", uname, nodename, cwd, start_time);
    }
    else
    {
        printf(GRN "%s@%s" RESET ":" BLU "%s" RESET "$ ", uname, nodename, cwd);
    }
    free(buf);
    free(cwd);
    fflush(stdout);
}