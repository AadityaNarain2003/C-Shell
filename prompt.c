#include "prompt.h"

Home init_prompt()
{
    Home h = (Home)malloc(sizeof(struct Prompt));
    h->current = (char *)malloc(sizeof(char) * 256);
    h->login= (char *)malloc(sizeof(char) * 256);
    h->host= (char *)malloc(sizeof(char) * 256);
    h->login=getlogin();
    gethostname(h->host, 256);
    getcwd(h->current, 256);
    return h;
}

void prompt(Home h)
{
    printf("\033[33m");
    
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s@%s:~", h->login,h->host);
    if (strcmp(h->current, cwd)==0)
    {
        printf(">");
    }
    else
    {
        for (int i = 6 + strlen(h->login); i < strlen(cwd); i++)
        {
            printf("%c", cwd[i]);
        }
        printf("> ");
    }

    printf("\033[0;37m");
}
