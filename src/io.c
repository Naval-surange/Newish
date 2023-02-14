#include "headers.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

char **getFiles(int *noFiles, char *starts_with)
{
    int no_files = 0;

    DIR *d;
    struct dirent *dir;

    d = opendir(getcwd(NULL, 0));

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strncmp(dir->d_name, starts_with, strlen(starts_with)) == 0)
            {
                no_files++;
            }
        }
        closedir(d);
    }

    char **files = (char **)malloc(sizeof(char *) * no_files);

    d = opendir(getcwd(NULL, 0));

    if (d)
    {
        int j = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if (strncmp(dir->d_name, starts_with, strlen(starts_with)) == 0)
            {
                files[j] = (char *)malloc(sizeof(char) * strlen(dir->d_name));
                strcpy(files[j], dir->d_name);
                if (dir->d_type == 4)
                {
                    strcat(files[j], "/");
                }
                j++;
            }
        }
        closedir(d);
    }
    *noFiles = no_files;
    return files;
}

char *get_first(char *str)
{
    char *starting = strrchr(str, ' ');
    if (starting == NULL)
    {
        starting = str;
    }
    else
    {
        starting++;
    }

    return starting;
}

void handleTab(char *inp, int *pt)
{
    char *starts_with = get_first(inp);
    int no_files = 0;
    char **files = getFiles(&no_files, starts_with);

    if (no_files == 0)
    {
        return;
    }
    else if (no_files == 1)
    {
        printf("\r");
        prompt();
        inp[strlen(inp) - strlen(starts_with)] = '\0';
        strcat(inp, files[0]);
        strcat(inp, " ");
        printf("%s", inp);
        *pt = strlen(inp);
    }
    else
    {
        printf("\n");
        for (int i = 0; i < no_files; i++)
        {
            if (files[i][strlen(files[i]) - 1] == '/')
            {
                printf(BLU "%s\t" RESET, files[i]);
            }
            else
            {
                printf("%s\t", files[i]);
            }
        }
        printf("\n");
        prompt();
        printf("%s", inp);
    }
}

char *handleIO(char *inp)
{
    char c;
    setbuf(stdout, NULL);
    enableRawMode();
    memset(inp, '\0', 1024);
    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                {
                    if (buf[0] == 91)
                    {
                        if (buf[1] == 65)
                        {
                            if (history.size != 0)
                            {
                                while (pt > 0)
                                {
                                    printf("\b \b");
                                    pt--;
                                }
                                char *newCmd = malloc(sizeof(char) * 1024);
                                strcpy(newCmd, history.cmd[history.size - 1]);
                                newCmd[strlen(newCmd) - 1] = '\0';
                                strcpy(inp, newCmd);
                                pt = strlen(newCmd);
                                printf("%s", newCmd);
                            }
                        }
                    }
                }
            }
            else if (c == 127)
            {
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {
                handleTab(inp, &pt);
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }
    disableRawMode();
    printf("\n");
    strcat(inp, "\n");
    return inp;
}
