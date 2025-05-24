#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>

#include "prompt.h"
#include "pastevents.h"
#include "peek.h"
#include "proclore.h"
#include "seek.h"
#include "warp.h"
#include "file.h"
#include "sig.h"
#include "Activities.h"
#include "network.h"
#include "fgbg.h"
#include "neonate.h"

typedef struct Child *Anish;

struct Child
{
    int *pids;
    char **names;
    int *validity;
    int curr;
};

char *trimSpaces(char *str);
Anish init_child();
void tokenizing(char *input, char **arguments, int *counter, const char *s);
int cmp_string(char *a);
void run_foreground_background(char *copy_input, Anish a,ListPtr lp);
void run_other_commands(char *copy_input,ListPtr lp);
void run_commands(char *arguments, PasteBin pb, Storage s, char *copy_input, Home h,ListPtr lp);
void file_checkers(char *arguments, PasteBin pb, Storage s, char *copy_input, Home h,ListPtr lp);
void check_background(Anish a);
void run_all(char *input, PasteBin pb, Storage s, Home h, Anish a,ListPtr lp);
void piping_maybe(char* input,char* copy_input, PasteBin pb, Storage s, Home h, Anish a, ListPtr lp);

#endif
