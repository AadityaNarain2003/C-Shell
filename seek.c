#include "seek.h"

int isDirectory_seek(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}
int isFile(const char *path)
{
    return !isDirectory_seek(path);
}

void explore_directory(char **arguments, char **match, int *matches, int d_flag, int f_flag, int e_flag, int no_flag)
{
    // printf("HERE\n");
    
    DIR *dr = opendir(".");
    if(dr==NULL)
    {
        return;
    }
    struct dirent *de;
    while ((de = readdir(dr)) != NULL)
    {
        //
        if (strncmp(arguments[0], de->d_name, strlen(arguments[0])) == 0)
        {
            
            if (isDirectory_seek(de->d_name) == 1 && d_flag == 1)
            {
                
                match[*matches] = (char *)malloc(sizeof(char) * 256);
                getcwd(match[*matches], 256);
                match[*matches][strlen(match[*matches])] = '/';
                int len = strlen(match[*matches]);
                for (int i = 0; i < strlen(de->d_name); i++)
                {
                    match[*matches][i + len] = de->d_name[i];
                }
                //printf("here ./%s\n", de->d_name);
                *matches = *matches + 1;
            }
            else if (isFile(de->d_name) == 1 && f_flag == 1)
            {
                match[*matches] = (char *)malloc(sizeof(char) * 256);
                getcwd(match[*matches], 256);
                match[*matches][strlen(match[*matches])] = '/';
                int len = strlen(match[*matches]);
                for (int i = 0; i < strlen(de->d_name); i++)
                {
                    match[*matches][i + len] = de->d_name[i];
                }
                *matches = *matches + 1;
            }
            else if (no_flag == 1)
            {
                // printf("NO FLAG\n");
                match[*matches] = (char *)malloc(sizeof(char) * 256);
                getcwd(match[*matches], 256);
                match[*matches][strlen(match[*matches])] = '/';
                int len = strlen(match[*matches]);
                for (int i = 0; i < strlen(de->d_name); i++)
                {
                    match[*matches][i + len] = de->d_name[i];
                }
                *matches = *matches + 1;
            }
        }
        if (isDirectory_seek(de->d_name) == 1 && de->d_name[0] != '.')
        {
            //printf("%s\n",de->d_name);
            chdir(de->d_name);
            explore_directory(arguments, match, matches, d_flag, f_flag, e_flag, no_flag);
            chdir("..");
        }
    }
}

void seeking1(char *prompt,char *loginname)
{
    char cwd[256];
    getcwd(cwd, 256);
    int len_cwd = strlen(cwd);

    if (prompt == NULL)
    {
        printf("Missing permissions for task!\n");
        return;
    }
    prompt[strlen(prompt)] = '\0';
    int d_flag = 0;
    int f_flag = 0;
    int e_flag = 0;
    int no_flag = 0;

    // to get all the data in a particular format
    char **arguments = (char **)malloc(sizeof(char *) * 3);
    int counter = 0;
    const char s[4] = " ";
    char *token = strtok(prompt, s);
    while (token != NULL)
    {
        if (strcmp(token, "-f") == 0)
        {
            f_flag = 1;
        }
        else if (strcmp(token, "-d") == 0)
        {
            d_flag = 1;
        }
        else if (strcmp(token, "-e") == 0)
        {
            e_flag = 1;
        }
        else
        {
            arguments[counter] = malloc(sizeof(strlen(token) + 1));
            strcpy(arguments[counter], token);

            counter = 1 + counter;
        }
        token = strtok(NULL, s);
    }

    if (f_flag == 1 && d_flag == 1)
    {
        printf("Invalid flags!\n");
        return;
    }
    if (f_flag == 0 && d_flag == 0)
    {
        no_flag = 1;
    }

    char **match = (char **)malloc(sizeof(char *) * 1024);
    int matches = 0;
    //now we come to execuet commands
    if (counter == 1 || (counter == 2 && strncmp(arguments[counter - 1], ".", 1) == 0))
    {

        explore_directory(arguments, match, &matches, d_flag, f_flag, e_flag, no_flag);
    }
    else if (counter == 2 && arguments[counter - 1][0] == '~')
    {
        //char *loginname =(char*)malloc(sizeof(char)*256);
       // loginname= getlogin();

        char fin[512];
        sprintf(fin, "/home/%s", loginname);
        //free(loginname);
        char cwd[256];
        getcwd(cwd, 256);
        chdir(fin);
        
        explore_directory(arguments, match, &matches, d_flag, f_flag, e_flag, no_flag);

 
        int len_cwd = strlen(fin);
        if (e_flag == 0)
        {      
            if (matches == 0)
            {
                printf("No matches found!\n");
            }
            for (int i = 0; i < matches; i++)
            {
                printf(".");
                for (int j = len_cwd; j < strlen(match[i]); j++)
                {
                    printf("%c", match[i][j]);
                }
                printf("\n");
            }
        }
        else
        {
            if (matches != 1)
            {
                // do nothing
            }
            else
            {
                if (d_flag == 1)
                { 
                    chdir(match[0]);
                    printf("%s\n", match[0]);
                    return;
                }
                if (f_flag == 1)
                {
                    char your_mom[1024];
                    FILE *fi = fopen(match[0], "r");
                    // need to add error for missing command
                    while (fgets(your_mom, 1024, fi))
                    {
                        printf("%s", your_mom);
                    }
                    chdir(cwd);
                    return;
                }
            }
        }

        chdir(cwd);
        return;

    }
    else if (counter == 2 && arguments[counter - 1][0] == '/')
    {

        char cwd[256];
        getcwd(cwd, 256);
        chdir(arguments[counter - 1]);

        explore_directory(arguments, match, &matches, d_flag, f_flag, e_flag, no_flag);
        int len_cwd = strlen(arguments[counter - 1]);
        if (e_flag == 0)
        {
            if (matches == 0)
            {
                printf("No matches found!\n");
            }
            for (int i = 0; i < matches; i++)
            {
                printf(".");
                for (int j = len_cwd; j < strlen(match[i]); j++)
                {
                    printf("%c", match[i][j]);
                }
                printf("\n");
            }
        }
        else
        {
            if (matches != 1)
            {
                // do nothing
            }
            else
            {
                if (d_flag == 1)
                {
                    chdir(match[0]);
                    printf("%s\n", match[0]);
                    return;
                }
                if (f_flag == 1)
                {
                    char your_mom[1024];
                    FILE *fi = fopen(match[0], "r");
                    // need to add error for missing command
                    while (fgets(your_mom, 1024, fi))
                    {
                        printf("%s", your_mom);
                    }
                    chdir(cwd);
                    return;
                }
            }
        }
        chdir("..");

        chdir(cwd);
        return;
    }
    else if (counter == 2)
    {
        char cwd[256];
        getcwd(cwd, 256);
        // need to go to the specific directory first and do the remaining
        DIR *dr = opendir(".");
        struct dirent *de;
        /*
        if(strncmp(arguments[counter - 1], "~", 1) == 0)
        {
            chdir("/home/aaditya");
            dr=opendir(".");
        }*/
        // now to delete the first two characters of folder name .. the ./
        int len = strlen(arguments[counter - 1]);
        int i = 2;
        for (; i < len + 1; i++)
        {
            arguments[counter - 1][i - 2] = arguments[counter - 1][i];
        }
        arguments[counter - 1][i] = '\0';
        while ((de = readdir(dr)) != NULL)
        {
            // now need to compare the name of the directories
            // printf("%s  %s\n", arguments[0], de->d_name);
            if (strcmp(arguments[0], de->d_name) == 0)
            {
                chdir(de->d_name);

                explore_directory(arguments, match, &matches, d_flag, f_flag, e_flag, no_flag);

                chdir("..");
            }
        }
        chdir(cwd);
    }
    else{}

    if (counter == 0)
    {
        printf("No match found!\n");
        return;
    }
    if (e_flag == 0)
    {
        if (matches == 0)
        {
            printf("No matches found!\n");
        }
        for (int i = 0; i < matches; i++)
        {
            printf(".");
            for (int j = len_cwd; j < strlen(match[i]); j++)
            {
                printf("%c", match[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        if (matches != 1)
        {
            // do nothing
        }
        else
        {
            if (d_flag == 1)
            {
                chdir(match[0]);
                printf("%s\n", match[0]);
            }
            if (f_flag == 1)
            {
                char your_mom[1024];
                FILE *fi = fopen(match[0], "r");
                // need to add error for missing command
                while (fgets(your_mom, 1024, fi))
                {
                    printf("%s", your_mom);
                }
            }
        }
    }
}
