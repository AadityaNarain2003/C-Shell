#ifndef _SIG_H_
#define _SIG_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void sendsig(int pid, int type);
void sendsig_util(char *new_token);
void handle_C(int num);
void handle_D(int num);
void handle_Z(int num);


#endif