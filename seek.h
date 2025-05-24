#ifndef SEEK_H_
#define SEEK_H_

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int isDirectory_seek(const char *path);
int isFile(const char *path);
void explore_directory(char **arguments, char **match, int *matches, int d_flag, int f_flag, int e_flag, int no_flag);
void seeking1(char *prompt,char *loginname);

#endif