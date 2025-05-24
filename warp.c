#include "warp.h"

Storage init_Warp()
{
    Storage s = (Storage)malloc(sizeof(struct Store));
    s->last = (char *)malloc(sizeof(char) * 256);

    char *loginname = getlogin();
    int length = 6 + strlen(loginname);
    char cwd[256];
    getcwd(cwd, 256);
    int i = 0;
    for (; i < length; i++)
    {
        s->last[i] = cwd[i];
    }
    s->last[i] = '\0';

    // this is to initialize the storage to the last value of path , starting at home directory

    return s;
}

void go_to_home(Storage s)
{
    // this part of code sends us back to home
    // also updates the value of s->last to the current data
    char *cwd;
    cwd = (char *)calloc(sizeof(char), 256);
    getcwd(cwd, 256);

    free(s->last);
    s->last = (char *)malloc(sizeof(char) * 256);
    strcpy(s->last, cwd);

    char *loginname = getlogin();
    int length = 6 + strlen(loginname);
    while (length != strlen(cwd))
    {
        chdir("..");
        free(cwd);
        cwd = (char *)calloc(sizeof(char), 256);
        getcwd(cwd, 256);
    }
}

void go_to_last(Storage s)
{
    char *cwd;
    cwd = (char *)calloc(sizeof(char), 256);
    getcwd(cwd, 256);

    chdir(s->last);

    free(s->last);
    s->last = (char *)malloc(sizeof(char) * 256);
    strcpy(s->last, cwd);
}

void go_back(Storage s)
{
    char *cwd;
    cwd = (char *)calloc(sizeof(char), 256);
    getcwd(cwd, 256);

    int e=chdir("..");
    if(e!=0)
    {
        
        return;
    }

    free(s->last);
    s->last = (char *)malloc(sizeof(char) * 256);
    strcpy(s->last, cwd);
}

void change_directory(Storage s, char *prompt)
{
    
    prompt[strlen(prompt)]='\0';
    //printf("|%s|\n",prompt);
    char *cwd;
    cwd = (char *)calloc(sizeof(char), 256);
    getcwd(cwd, 256);
    

    if(!chdir(prompt)==0)
        {}

    free(s->last);
    s->last = (char *)malloc(sizeof(char) * 256);
    strcpy(s->last, cwd);
}

void follow_absolute(Storage s,char* prompt)
{
    prompt[strlen(prompt)]='\0';
    //printf("|%s|",prompt);
    char *cwd;
    cwd = (char *)calloc(sizeof(char), 256);
    getcwd(cwd, 256);

    
    chdir(prompt);

    free(s->last);
    s->last = (char *)malloc(sizeof(char) * 256);
    strcpy(s->last, cwd);
}

void warp(Storage s, char *prompt)
{
    if(prompt[0]=='/')
    {   
        follow_absolute(s,prompt);
        return;
    }
    const char x[4] = "/";
    char *token = strtok(prompt, x);
    while (token != NULL)
    {
        if (strncmp(token, "-",1) == 0)
        {
            go_to_last(s);
            char c[256];
            getcwd(c, 256);
            printf("%s\n", c);
            break;
        }
        else if (strncmp(token, "..",2) == 0)
        {
            go_back(s);
        }
        else if (strncmp(token, "~",1) == 0)
        {
            go_to_home(s);
        }
        else if (strncmp(token, ".",1) == 0)
        {
            // do nothing
        }
        else
        {
            change_directory(s, prompt);
        }
        token = strtok(NULL, x);
    }
}

