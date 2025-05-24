#ifndef _NEONATE_H_
#define _NEONATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>

void delay(int number_of_seconds);
void disableRawMode_c();
void enableRawMode_c();
int kbhit();
void neonate_karde(char *time_c);

#endif