#ifndef PROCLORE_H_
#define PROCLORE_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct STATS* INFO;

struct STATS
{
    char* state;
    char* gid;
    char* vm;
    char* path;
};

int details_curr_process();
void proc_no_prompt();
void proc_with_prompt(int pid);
void proc(char* prompt);


#endif