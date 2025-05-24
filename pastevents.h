#ifndef PASTEVENTS_H_
#define PASTEVENTS_H_

#define SIZE 15
#define MAX_SIZE 1600


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>


typedef struct Paste* PasteBin;

struct Paste
{
    int curr_size;
    int curr_pos;
    char** commands;
};

PasteBin initPasteBin();
PasteBin file_initPasteBin();
void file_writePasteBin(PasteBin pb);
int insert(PasteBin pb, char* txt);
void print_BIN(PasteBin p);
void purge(PasteBin pb);
char* execute(PasteBin pb,int index);
void printAll(PasteBin pb);
void pastevent_caller(PasteBin pb, char* input);

#endif

/*
the format of the file is majorly
number of elements
current position
all the elements

*/