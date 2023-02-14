#ifndef _HEADERS_H_
#define _HEADERS_H_

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/signal.h>
#include <time.h>
#include <ctype.h>

#include "parser.h"
#include "prompt.h"
#include "history.h"
#include "signal.h"
#include "process.h"

#define MAX_ARGS 256

extern char *HOME;
extern time_t start_time;

#endif