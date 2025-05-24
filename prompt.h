#ifndef __PROMPT_H
#define __PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct Prompt* Home;

struct Prompt
{
    char* current;
    char* login;
    char* host;
};

Home init_prompt();
void prompt(Home h);

#endif