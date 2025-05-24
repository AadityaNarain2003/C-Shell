#include "file.h"

void check_flag_input(Files f,char* input)
{
    int l=strlen(input)-1;
    for(int i=1;i<l;i++)
    {
        if( input[i]=='<')
        {
            f->flag_input=i;
            return;
        }
        
    }
}
void check_flag_append(Files f, char* input)
{
    int l=strlen(input)-1;
    for(int i=1;i<l;i++)
    {
        if(input[i]=='>' && input[i+1]=='>')
        {
            f->flag_append=i;
            return;
        }
        
    }
}
void check_flag_output(Files f, char* input)
{
    int l=strlen(input)-1;
    for(int i=1;i<l;i++)
    {
        if (input[i]=='>' && input[i+1]!='>' && input[i-1]!='>')
        {
            f->flag_output=i;
            return;
        }
        
        
    }
}

void pipe_cnt(Files f,char* input)
{
    int l=strlen(input);
    for(int i=0;i<l;i++)
    {
        if(input[i]=='|')
        {
            f->pipe_count++;
        }
    }
}


Files init_file_systems()
{
    Files f=(Files)malloc(sizeof(struct File_System));
    f->flag_append=-1;
    f->flag_input=-1;
    f->flag_output=-1;
    f->pipe_count=0;
    return f;
}