#include "headers.h"
#include "history.h"

History history;
History lastDir;

void init_history(History *history)
{
    history->cmd = (char **)malloc(sizeof(char *) * 10);
    history->capacity = 10;
    history->size = 0;
}

void add_history(History *history, char *cmd)
{
    if (history->size > 0)
    {
        if (strcmp(history->cmd[history->size - 1], cmd) == 0 || strcmp("", cmd) == 0 || strcmp("\n", cmd) == 0)
        {
            return;
        }
    }

    if (history->size == history->capacity)
    {
        history->capacity *= 2;
        history->cmd = (char **)realloc(history->cmd, sizeof(char *) * history->capacity);
    }
    history->cmd[history->size] = (char *)malloc(sizeof(char) * strlen(cmd));
    strcpy(history->cmd[history->size], cmd);
    history->size++;
}

void free_history(History *history)
{
    for (int i = 0; i < history->size; i++)
    {
        free(history->cmd[i]);
    }
    free(history->cmd);
}

void print_history(History *history, char **args, int len)
{
    int l = 10;
    if (len == 2)
    {
        l = atoi(args[1]);
    }

    if (l > history->size)
    {
        l = history->size;
    }
    for (int i = history->size - l; i < history->size; i++)
    {
        printf("[%d] %s", i + 1, history->cmd[i]);
    }
}

void write_history(History *history)
{
    char *path = (char *)malloc(sizeof(char) * 100);
    strcpy(path, HOME);
    strcat(path, "/src/history.txt");
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf(RED "write_history: error opening file\n" RESET);
        return;
    }

    for (int i = 0; i < history->size; i++)
    {
        fprintf(fp, "%s", history->cmd[i]);
    }
    fclose(fp);
}

void read_history()
{
    char *path = (char *)malloc(sizeof(char) * 100);
    strcpy(path, HOME);
    strcat(path, "/src/history.txt");

    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        add_history(&history, line);
    }
    fclose(fp);
}