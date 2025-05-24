#include "sig.h"

void sendsig(int pid, int type)
{
    if(kill(pid,type))
    {
        printf("Sent signal %d to process with pid %d\n",type,pid);
        return;
    }
    else
    {
        printf("No Such Process Found\n");
    }
}

void sendsig_util(char *new_token)
{
    new_token[strlen(new_token)]='\0';
    //printf("|%s|\n",new_token);
    char* token=strtok(new_token," ");
    int pid=atoi(token);
    //printf("%s\n",token);
    token=strtok(NULL,"\0");
    //printf("%s\n",token);
    int type=atoi(token);
    sendsig(pid,type);
}
 
void handle_C(int signo) 
{ 
  return;
} 
void handle_D(int num)
{
    int pid=tcgetpgrp(STDIN_FILENO);
    kill(pid,9);
}

void handle_Z(int num)
{
    exit(0);
}