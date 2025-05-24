#include "fgbg.h"

void fg_try(char* num)
{
    int a=atoi(num);
    
    int x=kill(a,SIGCONT);
    if(x!=0)
    {
        printf("No Such Process Found\n");  
    }
    else
    {
    setpgid(a, 0);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);   
    tcsetpgrp(0, a);

    int t;
    waitpid(a, &t, WUNTRACED);
    
    tcsetpgrp(0, getpgid(0));    
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    }
}

void bg_try(char* num)
{
    int a=atoi(num);
    
    int x=kill(a,SIGCONT);
    if(x!=0)
    {
        printf("No Such Process Found\n");  
    }
}