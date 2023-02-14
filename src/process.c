#include "process.h"
#include "headers.h"

Processes plist;

void init_processes(Processes *Processes)
{
    Processes->size = 0;
    Processes->capacity = 10;
    Processes->names = malloc(sizeof(char *) * Processes->capacity);
    Processes->bg = malloc(sizeof(int) * Processes->capacity);
    Processes->status= malloc(sizeof(int) * Processes->capacity);
    Processes->pids = malloc(sizeof(int) * Processes->capacity);
}
void add_processes(Processes *Processes, char *name, int pid,int bg)
{
    if (Processes->size == Processes->capacity)
    {
        Processes->capacity *= 2;
        Processes->names = realloc(Processes->names, sizeof(char *) * Processes->capacity);
        Processes->bg = realloc(Processes->bg, sizeof(int) * Processes->capacity);
        Processes->status = realloc(Processes->status, sizeof(int) * Processes->capacity);
        Processes->pids = realloc(Processes->pids, sizeof(int) * Processes->capacity);
    }
    Processes->names[Processes->size] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(Processes->names[Processes->size], name);
    Processes->bg[Processes->size] = bg;
    Processes->status[Processes->size] = 1;
    Processes->pids[Processes->size] = pid;
    Processes->size++;
}

void remove_processes(Processes *Processes, int index)
{    
    free(Processes->names[index]);
    for (int i = index; i < Processes->size - 1; i++)
    {
        Processes->names[i] = Processes->names[i + 1];
        Processes->bg[i] = Processes->bg[i + 1];
        Processes->status[i] = Processes->status[i + 1];
        Processes->pids[i] = Processes->pids[i + 1];
    }
    Processes->size--;
}

int get_by_pid(Processes *Processes, int pid)
{
    for (int i = 0; i < Processes->size; i++)
    {
        if (Processes->pids[i] == pid)
        {
            return i;
        }
    }
    return -1;
}