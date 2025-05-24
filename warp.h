#ifndef WARP_H_
#define WARP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct Store* Storage;

struct Store
{
    char* last;
};

Storage init_Warp();
void go_to_home(Storage s);
void go_to_last(Storage s);
void go_back(Storage s);
void change_directory(Storage s, char *prompt);
void follow_absolute(Storage s,char* prompt);
void warp(Storage s, char *prompt);

#define MAX_ARGS 100

#endif