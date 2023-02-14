#include "headers.h"

int containsOnly(char *str, char *delimiter)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (strchr(delimiter, str[i]) == NULL)
        {
            return 0;
        }
        i++;
    }
    return 1;
}
char *multi_tok(char *input, char *delimiter)
{
    static char *string;
    if (input != NULL)
        string = input;

    if (string == NULL)
        return string;

    char *end = strstr(string, delimiter);
    if (end == NULL)
    {
        char *temp = string;
        string = NULL;
        return temp;
    }

    char *temp = string;

    *end = '\0';
    string = end + strlen(delimiter);
    return temp;
}

char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if (str == NULL)
    {
        return NULL;
    }
    if (str[0] == '\0')
    {
        return str;
    }

    len = strlen(str);
    endp = str + len;

    while (isspace((unsigned char)*frontp))
    {
        ++frontp;
    }
    if (endp != frontp)
    {
        while (isspace((unsigned char)*(--endp)) && endp != frontp)
        {
        }
    }

    if (frontp != str && endp == frontp)
        *str = '\0';
    else if (str + len - 1 != endp)
        *(endp + 1) = '\0';

    endp = str;
    if (frontp != str)
    {
        while (*frontp)
        {
            *endp++ = *frontp++;
        }
        *endp = '\0';
    }

    return str;
}

void removeuseless(char *c)
{
    size_t sl = strlen(c);

    if (sl > 0 && c[sl - 1] == '\n')
    {
        c[sl - 1] = '\0';
    }
    c = trim(c);
}
