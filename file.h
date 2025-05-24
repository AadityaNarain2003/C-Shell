#ifndef _FILE_H_
#define _FILE_H_

#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

//flag_dir specifies what to do with a particular flag
/*
0 means nothing
1 means use 
*/
typedef struct File_System* Files;

struct File_System
{
    int flag_input;
    int flag_output;
    int flag_append;
    int pipe_count;
};

Files init_file_systems();

void pipe_cnt(Files f,char* input);

void check_flag_input(Files f,char* input);
void check_flag_append(Files f, char* input);
void check_flag_output(Files f, char* input);

#endif