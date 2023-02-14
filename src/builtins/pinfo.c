#include "../headers.h"
#include "pinfo.h"

void pinfo(char **args, int len)
{
    if (len > 2)
    {
        printf("pinfo: too many arguments\n");
        return;
    }
    pid_t pid = getpid();
    if (len == 2)
    {
        pid = atoi(args[1]);
    }
    char *process_path = malloc(sizeof(char) * 1024);
    sprintf(process_path, "/proc/%d/stat", pid);
    FILE *fp = fopen(process_path, "r");
    if (fp == NULL)
    {
        printf("pinfo: no such process\n");
        return;
    }
    char *buff = (char *)malloc(sizeof(char) * 1024 * 4);
    fgets(buff, 1024 * 4, fp);

    char *token = strtok(buff, " ");
    int i = 0;
    char *state = malloc(sizeof(char) * 10);
    long int memory = 0;
	pid_t pgrp_id = 0;

    while (token != NULL)
    {
        if (i == 2)
            strcpy(state, token);
        if(i == 4)
            pgrp_id = atoi(token);
        if (i == 22)
            memory = atoi(token);
        token = strtok(NULL, " ");
        i++;
    }
    char state_activity = (pgrp_id==tcgetpgrp(0)) ? '+':'-';

    char *exec_path = malloc(sizeof(char) * 1024 * 4);
    sprintf(process_path, "/proc/%d/exe", pid);

    readlink(process_path, exec_path, 1024 * 4);
    printf("pid -- %d\n", pid);
    printf("Process Status -- %s%c\n", state,state_activity);
    printf("memory -- %ldB\n", memory);
    printf("Executable Path -- %s\n", exec_path);
    fclose(fp);
    free(process_path);
    free(buff);
    free(state);
    free(exec_path);

    return;
}