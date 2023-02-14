#include "../headers.h"
#include "ls.h"

int compare(const void *a, const void *b)
{
    return strcasecmp(*(const char **)a, *(const char **)b);
}

char *replaceTilda(const char *s)
{
    char *oldW = "~";
    char *newW = HOME;
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

void ls(char **args, int len)
{

    char **args_wo_flag = (char **)malloc(sizeof(char *) * len);
    int no_args_wo_flag = 0;

    int all = 0, extra = 0;

    for (int i = 0; i < len; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] != 'a' && args[i][j] != 'l')
                {
                    printf(RED "ls: invalid option -- '%c'\n" RESET, args[i][j]);
                    return;
                }
                if ((args[i][j] == 'a' && all) || (args[i][j] == 'l' && extra))
                {
                    printf(RED "ls: option '%c' is already set\n" RESET, args[i][j]);
                    return;
                }
                if (args[i][j] == 'a')
                {
                    all = 1;
                }
                if (args[i][j] == 'l')
                {
                    extra = 1;
                }
            }
        }
        else
        {
            args_wo_flag[no_args_wo_flag] = replaceTilda(args[i]);
            no_args_wo_flag++;
        }
    }

    if (no_args_wo_flag == 1)
    {
        no_args_wo_flag++;
        args_wo_flag = (char **)realloc(args, sizeof(char *) * no_args_wo_flag);
        args_wo_flag[1] = ".";
    }

    for (int i = 1; i < no_args_wo_flag; i++)
    {
        int no_files = 0;

        DIR *d;
        struct dirent *dir;
        d = opendir(args_wo_flag[i]);
        if (no_args_wo_flag > 2)
            printf("%s:\n", args_wo_flag[i]);

        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.' || all)
                {
                    no_files++;
                }
            }
            closedir(d);
        }
        else
        {
            printf(RED "%s: No such file or directory\n" RESET, args_wo_flag[i]);
            return;
        }

        char **files = (char **)malloc(sizeof(char *) * no_files);

        d = opendir(args_wo_flag[i]);

        if (d)
        {
            int j = 0;
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.' || all)
                {
                    files[j] = (char *)malloc(sizeof(char) * strlen(dir->d_name) + 2);
                    strcpy(files[j], dir->d_name);
                    j++;
                }
            }
            closedir(d);
        }

        qsort(files, no_files, sizeof(char *), compare);

        if (!extra)
        {
            for (int j = 0; j < no_files; j++)
            {
                struct stat fileStat;
                char *path = (char *)malloc(sizeof(char) * (strlen(args_wo_flag[i]) + strlen(files[j]) + 2));
                strcpy(path, args_wo_flag[i]);
                strcat(path, "/");
                strcat(path, files[j]);

                if (stat(path, &fileStat) < 0)
                {
                    printf("Error: Cannot access %s\n", path);
                    return;
                }

                if (S_ISDIR(fileStat.st_mode))
                {
                    printf(BLU "%s\t" RESET, files[j]);
                }
                else if (fileStat.st_mode & S_IXOTH || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXUSR)
                {
                    printf(GRN "%s\t" RESET, files[j]);
                }
                else
                {
                    printf(RESET "%s\t" RESET, files[j]);
                }
            }
        }
        else
        {
            for (int j = 0; j < no_files; j++)
            {
                struct stat fileStat;
                char *path = (char *)malloc(sizeof(char) * (strlen(args_wo_flag[i]) + strlen(files[j]) + 2));
                strcpy(path, args_wo_flag[i]);
                strcat(path, "/");
                strcat(path, files[j]);

                if (stat(path, &fileStat) < 0)
                {
                    printf("Error: Cannot access %s\n", path);
                    return;
                }

                printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                printf(" %ld ", fileStat.st_nlink);
                struct passwd *pw = getpwuid(fileStat.st_uid);
                struct group *gr = getgrgid(fileStat.st_gid);
                printf("%s %s ", pw->pw_name, gr->gr_name);
                printf("%ld\t", fileStat.st_size);
                char date[20];
                strftime(date, 20, "%b %d %H:%M", localtime(&fileStat.st_mtime));
                printf("%s\t", date);
                printf("%s \n", files[j]);
            }
        }
        printf("\n");
        if (no_args_wo_flag > 2)
            printf("\n");
        return;
    }
    return;
}
