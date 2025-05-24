#include "proclore.h"

int details_curr_process()
{
    return getpid();
}

void proc_no_prompt()
{
    int pid = details_curr_process();
    //printf("pid: %d\n",pid);
    char path[1024];

    INFO info=(INFO)malloc(sizeof(struct STATS));
    info->state=(char*)malloc(sizeof(char)*16);
    info->gid=(char*)malloc(sizeof(char)*16);
    info->vm=(char*)malloc(sizeof(char)*16);
    info->path=(char*)malloc(sizeof(char)*512);

    sprintf(path, "/proc/%d/status", pid);
    FILE* fi = fopen(path, "r");
    if(fi==NULL)
    {
        printf("PROCESS doesnt exist\n");
        return;
    }
    
    int tgpid;

    char buffer[1024];
    while(fgets(buffer,1024,fi))
    {
        //printf("%s\n",buffer);
        if(strncmp(buffer,"State:",6)==0)
        {
            sscanf(buffer+7,"%s",info->state);
        }
        if(strncmp(buffer,"PPid:",5)==0)
        {
            sscanf(buffer+6,"%s",info->gid);
        }
        if(strncmp(buffer,"VmSize:",7)==0)
        {
            sscanf(buffer+12,"%s",info->vm);
        }

        tgpid=tcgetpgrp(STDIN_FILENO);

    }
    

    char exec[1024];
    sprintf(exec, "/proc/%d/exe", pid);
    realpath(exec,info->path);

    printf("pid : %d\n",pid);
    printf("process status : %s",info->state);
    if(tgpid==pid)
    {
        printf("+\n");
    }
    else
    {
        printf("\n");
    }
    printf("Process Group : %s\n",info->gid);
    printf("Virtual memory : %s\n",info->vm);
    printf("executable path : %s\n",info->path);

    
}

void proc_with_prompt(int pid)
{

    char path[1024];

    INFO info=(INFO)malloc(sizeof(struct STATS));
    info->state=(char*)malloc(sizeof(char)*16);
    info->gid=(char*)malloc(sizeof(char)*16);
    info->vm=(char*)malloc(sizeof(char)*16);
    info->path=(char*)malloc(sizeof(char)*512);

    int tgpid;
    sprintf(path, "/proc/%d/status", pid);
    FILE* fi = fopen(path, "r");
    if(fi==NULL)
    {
        printf("PROCESS doesnt exist\n");
        return;
    }
    
    char buffer[1024];
    while(fgets(buffer,1024,fi))
    {
        //printf("%s\n",buffer);
        if(strncmp(buffer,"State:",6)==0)
        {
            sscanf(buffer+7,"%s",info->state);
        }
        if(strncmp(buffer,"PPid:",5)==0)
        {
            sscanf(buffer+6,"%s",info->gid);
        }
        if(strncmp(buffer,"VmSize:",7)==0)
        {
            sscanf(buffer+12,"%s",info->vm);
        }
        
         tgpid=tcgetpgrp(STDIN_FILENO);



    }
    

    char exec[1024];
    sprintf(exec, "/proc/%d/exe", pid);
    realpath(exec,info->path);

    printf("pid : %d\n",pid);
    printf("process status : %s",info->state);
    if(tgpid==pid)
    {
        printf("+\n");
    }
    else
    {
        printf("\n");
    }
    printf("Process Group : %s\n",info->gid);
    printf("Virtual memory : %s\n",info->vm);
    printf("executable path : %s\n",info->path);

    
}

void proc(char* prompt)
{
    if(prompt==NULL)
    {
        proc_no_prompt();
    }
    else
    {
        int x=atoi(prompt);
        proc_with_prompt(x);
    }
}

