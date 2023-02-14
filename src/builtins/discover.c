#include "discover.h"
#include "../headers.h"

char *replaceChar(const char *s)
{
    char *oldW = "\"";
    char *newW = "";
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

int discover_util(char *dirName, char *fileName, int search, int dir_f, int file_f)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirName);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            struct stat fileInfo;
            char *path = (char *)malloc(sizeof(char) * (strlen(dirName) + strlen(dir->d_name) + 2));
            strcpy(path, dirName);
            strcat(path, "/");
            strcat(path, dir->d_name);
            stat(path, &fileInfo);

            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            if (search)
            {
                if (strcmp(dir->d_name, fileName) == 0)
                {
                    if (dir_f && S_ISDIR(fileInfo.st_mode))
                        printf(GRN "%s\n" RESET, path);
                    else if (file_f && S_ISREG(fileInfo.st_mode))
                        printf("%s\n", path);
                    else if (!dir_f && !file_f)
                    {
                        if (S_ISDIR(fileInfo.st_mode))
                            printf(GRN "%s\n" RESET, path);
                        else if (S_ISREG(fileInfo.st_mode))
                            printf("%s\n", path);
                    }
                    return 1;
                }
            }
            else
            {
                if (dir_f && S_ISDIR(fileInfo.st_mode))
                    printf(GRN "%s\n" RESET, path);
                else if (file_f && S_ISREG(fileInfo.st_mode))
                    printf("%s\n", path);
                else if (!dir_f && !file_f)
                {
                    if (S_ISDIR(fileInfo.st_mode))
                        printf(GRN "%s\n" RESET, path);
                    else if (S_ISREG(fileInfo.st_mode))
                        printf("%s\n", path);
                }
            }

            if (S_ISDIR(fileInfo.st_mode))
                discover_util(path, fileName, search, dir_f, file_f);

            free(path);
        }
        closedir(d);
    }
    else
    {
        printf(RED "%s: No such file or directory\n" RESET, dirName);
        return 0;
    }
    return 0;
}

void discover(char **args, int len)
{
    if (len == 1)
    {
        args = (char **)realloc(args, sizeof(char *) * 2);
        args[1] = (char *)malloc(sizeof(char) * 2);
        strcpy(args[1], ".");
        len++;
    }

    char **args_wo_flag = (char **)malloc(sizeof(char *) * len);
    int no_args_wo_flag = 0;

    int file_f = 0, dir_f = 0;

    for (int i = 0; i < len; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] != 'f' && args[i][j] != 'd')
                {
                    printf(RED "discover: invalid option -- '%c'\n" RESET, args[i][j]);
                    return;
                }
                if ((args[i][j] == 'f' && file_f) || (args[i][j] == 'd' && dir_f))
                {
                    printf(RED "discover: option '%c' is already set\n" RESET, args[i][j]);
                    return;
                }
                if (args[i][j] == 'f')
                {
                    file_f = 1;
                }
                if (args[i][j] == 'd')
                {
                    dir_f = 1;
                }
            }
        }
        else
        {
            args_wo_flag[no_args_wo_flag] = args[i];
            no_args_wo_flag++;
        }
    }

    int search = 0;
    if (no_args_wo_flag == 1)
    {
        args_wo_flag = (char **)realloc(args_wo_flag, sizeof(char *) * 2);
        args_wo_flag[1] = (char *)malloc(sizeof(char) * 2);
        strcpy(args_wo_flag[1], ".");
        no_args_wo_flag++;
    }
    if (no_args_wo_flag == 3)
    {
        search = 1;
    }
    char *fileName;
    if (search)
    {
        fileName = replaceChar(args_wo_flag[2]);
    }
    discover_util(args_wo_flag[1], fileName, search, dir_f, file_f);
    printf("\n");
}
