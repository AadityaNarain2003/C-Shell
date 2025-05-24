#ifndef PEEK_H_
#define PEEK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "warp.h"


#define MAX_ARGS 100

int isDirectory(struct stat stat);
void fileType(struct stat stat);
void printLine(struct stat stat, struct dirent *de);
void peek(Storage warp,char *prompt);


#endif