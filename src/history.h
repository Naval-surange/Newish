#ifndef _HISTORY_H_
#define _HISTORY_H_

typedef struct History
{
    char **cmd;
    int pid;
    int size;
    int capacity;
} History;

void init_history(History *history);
void add_history(History *history, char *cmd);
void free_history(History *history);
void print_history(History *history, char** args,int len);
void write_history(History *history);
void read_history();

extern History history;
extern History lastDir;

#endif