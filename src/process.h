#ifndef _PROCESS_H_
#define _PROCESS_H_

typedef struct Processes
{
    char **names;
    int *pids;
    int *bg;
    int *status;
    int size;
    int capacity;
} Processes;

void init_processes(Processes *Processes);
void add_processes(Processes *Processes, char *name, int pid,int bg);
void remove_processes(Processes *Processes, int index);

int get_by_pid(Processes *Processes, int pid);

extern Processes plist;

#endif