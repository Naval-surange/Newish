#include "headers.h"
#include "execute.h"
#include "utils.h"
#include "io.h"

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

    if (tokens[i - 1][strlen(tokens[i - 1]) - 1] == '\n')
    {
        int strLen = strlen(tokens[i - 1]);
        tokens[i - 1][strLen - 1] = '\0';
    }
    *count = i;

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

    if (tokens[i - 1][strlen(tokens[i - 1]) - 1] == '\n')
    {
        int strLen = strlen(tokens[i - 1]);
        tokens[i - 1][strLen - 1] = '\0';
    }
    *count = i;

    return tokens;
}

void run(char *command, int bg)
{

    int noArgs = 0;
    char **args;
    args = tokenize(command, &noArgs, " \t");
    char **newArgs = malloc(sizeof(char *) * (1024));

    execute(args, noArgs, bg);
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
        out_file = open(args[1], O_RDWR | O_CREAT | O_APPEND, 0644);
    }

    int noArgs2 = 0;
    char **args2;
    args2 = tokenize(args[0], &noArgs2, ">");

    if (noArgs2 > 1)
    {
        removeuseless(args2[1]);
        out_file = open(args2[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    }

    int noArgs3 = 0;
    char **args3;
    args3 = tokenize(args2[0], &noArgs3, "<");
    int in_file = dup(fileno(stdin));
    int save_in = dup(fileno(stdin));

    if (noArgs3 > 1)
    {
        removeuseless(args3[1]);
        in_file = open(args3[1], O_RDWR | O_CREAT | O_APPEND, 0644);
    }

    dup2(out_file, fileno(stdout));
    dup2(in_file, fileno(stdin));

    run(args[0], bg);

    dup2(save_out, fileno(stdout));
    dup2(save_in, fileno(stdin));

    close(save_out);
    close(save_in);
    close(out_file);
    close(in_file);
}

void handelPipe(char *command, int bg)
{
    int noArgs = 0;
    char **args;
    args = multi_tokenize(command, &noArgs, "|");

    if (noArgs == 1)
    {
        handelRedirection(args[noArgs - 1], bg);
        return;
    }

    int backupout = dup(fileno(stdout));
    int backupin = dup(fileno(stdin));

    for (int i = 0; i < noArgs - 1; i++)
    {
        int piped_fdes[2];
        if (pipe(piped_fdes) < 0)
        {
            perror("Error in creating a pipe");
            break;
        }

        if (dup2(piped_fdes[1], STDOUT_FILENO) < 0)
        {
            perror("Error in dup2()");
        }

        handelRedirection(args[i], bg);

        if (dup2(piped_fdes[0], STDIN_FILENO) < 0)
        {
            perror("Error in dup2()");
        }
        close(piped_fdes[1]);
    }

    dup2(backupout, STDOUT_FILENO);
    handelRedirection(args[noArgs - 1], bg);
    dup2(backupin, STDIN_FILENO);
}

void parse(char *line)
{
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
            handelPipe(allCommands[j], bg);
        }
    }
}