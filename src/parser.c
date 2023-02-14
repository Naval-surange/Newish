#include "headers.h"
#include "execute.h"

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

char **tokenize(char *line, int *count, char *delimiter)
{
    if (containsOnly(line, delimiter) || containsOnly(line, " "))
    {
        *count = 0;
        return NULL;
    }
    *count = 0;
    char **tokens = malloc(sizeof(char *) * (1024));
    char *token = strtok(line, delimiter);
    int i = 0;
    while (token != NULL)
    {
        tokens[i] = token;
        token = strtok(NULL, delimiter);
        i++;
    }
    tokens[i] = NULL;
    *count = i;

    if (tokens[i - 1][strlen(tokens[i - 1]) - 1] == '\n')
    {
        int strLen = strlen(tokens[i - 1]);
        tokens[i - 1][strLen - 1] = '\0';
    }

    return tokens;
}

char **multi_tokenize(char *line, int *count, char *delimiter)
{
    if (containsOnly(line, delimiter) || containsOnly(line, " "))
    {
        *count = 0;
        return NULL;
    }
    *count = 0;
    char **tokens = malloc(sizeof(char *) * (1024));
    char *token = multi_tok(line, delimiter);
    int i = 0;
    while (token != NULL)
    {
        tokens[i] = token;
        token = multi_tok(NULL, delimiter);
        i++;
    }
    tokens[i] = NULL;
    *count = i;

    if (tokens[i - 1][strlen(tokens[i - 1]) - 1] == '\n')
    {
        int strLen = strlen(tokens[i - 1]);
        tokens[i - 1][strLen - 1] = '\0';
    }

    return tokens;
}

void run(char *command, int bg)
{
    int noArgs = 0;
    char **args;
    args = tokenize(command, &noArgs, " \t");

    execute(args, noArgs, bg);
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

void handelRedirection(char *command, int bg)
{

    int out_file = dup(fileno(stdout));
    int save_out = dup(fileno(stdout));

    int noArgs = 0;
    char **args;
    args = multi_tokenize(command, &noArgs, ">>");

    if (noArgs > 1)
    {
        removeuseless(args[1]);
        out_file = open(args[1], O_RDWR | O_CREAT | O_APPEND, 0600);
    }

    int noArgs3 = 0;
    char **args3;
    args3 = tokenize(args[0], &noArgs3, ">");

    if (noArgs3 > 1)
    {
        removeuseless(args3[1]);
        out_file = open(args3[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    }

    int noArgs2 = 0;
    char **args2;
    args2 = tokenize(args3[0], &noArgs2, "<");
    int in_file = dup(fileno(stdin));
    int save_in = dup(fileno(stdin));

    if (noArgs2 > 1)
    {
        removeuseless(args2[1]);
        in_file = open(args2[1], O_RDWR | O_CREAT | O_APPEND, 0600);
    }

    dup2(out_file, fileno(stdout));
    dup2(in_file, fileno(stdin));
    run(args[0], bg);
    dup2(save_out, fileno(stdout));
    dup2(save_in, fileno(stdin));
    close(save_out);
}

void parse(char *line)
{
    line = (char *)realloc(line, sizeof(char) * (strlen(line) + 3));
    strcat(line, " ;");

    int noCommands = 0;
    char **commands;
    commands = tokenize(line, &noCommands, ";");

    for (int i = 0; i < noCommands; i++)
    {
        int noAllCommands = 0;
        char **allCommands;
        allCommands = tokenize(commands[i], &noAllCommands, "&");

        for (int j = 0; j < noAllCommands; j++)
        {
            int bg = (j != noAllCommands - 1);

            // int noArgs = 0;
            // char **args;
            //  args = tokenize(allCommands[j], &noArgs, " \t");

            // if (j == noAllCommands - 1)
            //     execute(args, noArgs, 0);
            // else
            //     execute(args, noArgs, 1);

            if (j == noAllCommands - 1)
                handelRedirection(allCommands[j], 0);
            else
                handelRedirection(allCommands[j], 1);
        }
    }
}