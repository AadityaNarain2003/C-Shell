#include "peek.h"
#include "warp.h"

int isDirectory(struct stat stat)
{
    return S_ISDIR(stat.st_mode);
}
void fileType(struct stat stat)
{
    if (isDirectory(stat))
    {
        printf("\033[0;34m");
        return;
    }

    if (stat.st_mode & S_IXUSR)
    {
        printf("\033[0;32m");
    }
    else
    {
        printf("\033[0;37m");
    }
}

void printLine(struct stat stat, struct dirent *de)
{
    // printf("here\n");
    char perms[10];
    perms[0] = stat.st_mode & S_IRUSR ? 'r' : '-';
    perms[1] = stat.st_mode & S_IWUSR ? 'w' : '-';
    perms[2] = stat.st_mode & S_IXUSR ? 'x' : '-';
    perms[3] = stat.st_mode & S_IRGRP ? 'r' : '-';
    perms[4] = stat.st_mode & S_IWGRP ? 'w' : '-';
    perms[5] = stat.st_mode & S_IXGRP ? 'x' : '-';
    perms[6] = stat.st_mode & S_IROTH ? 'r' : '-';
    perms[7] = stat.st_mode & S_IWOTH ? 'w' : '-';
    perms[8] = stat.st_mode & S_IXOTH ? 'x' : '-';

    struct passwd *pw = getpwuid(stat.st_uid);
    struct group *gr = getgrgid(stat.st_gid);

    char *date = (char *)malloc(sizeof(char) * 30);
    date = ctime(&(stat.st_ctime));
    char *fix_date = (char *)malloc(sizeof(char) * 15);
    for (int i = 4; i < 16; i++)
    {
        fix_date[i - 4] = date[i];
    }
    fix_date[12] = '\0';

    printf("%s %ld %s\t%s\t%ld\t%s\t%s\n", perms, stat.st_nlink, pw->pw_name, gr->gr_name, stat.st_size, fix_date, de->d_name);
}

void peek(Storage warp, char *prompt)
{
    if (prompt == NULL)
    {
        char cwd[256];
        getcwd(cwd, 256);
        struct dirent *de;

        DIR *dr = opendir(".");
        while ((de = readdir(dr)) != NULL)
        {
            struct stat s;
            stat(de->d_name, &s);
            fileType(s);
            if(de->d_name[0]!='.')
            {
                printf("%s\n", de->d_name);
            }
        }
        printf("\033[0;37m");
        closedir(dr);
        return;
    }
    prompt[strlen(prompt)] = '\0';
    int a_flag = 0;
    int l_flag = 0;

    // to get all the data in a particular format
    char **arguments = (char **)malloc(sizeof(char *) * MAX_ARGS);
    int counter = 0;
    const char s[4] = " ";
    char *token = strtok(prompt, s);
    while (token != NULL)
    {
        if (strcmp(token, "-a") == 0)
        {
            a_flag = 1;
        }
        else if (strcmp(token, "-l") == 0)
        {
            l_flag = 1;
        }
        else if (strcmp(token, "-al") == 0 || strcmp(token, "-la") == 0)
        {
            a_flag = 1;
            l_flag = 1;
        }
        else
        {
            // printf("token is %s with length %ld\n",token,strlen(token));
            arguments[counter] = (char *)malloc(sizeof(char) * 512);
            strcpy(arguments[counter], token);

            counter = 1 + counter;
        }
        token = strtok(NULL, s);
    }

    if (counter == 0)
    {
        char cwd[256];
        getcwd(cwd, 256);
        struct dirent *de;

        DIR *dr = opendir(".");
        while ((de = readdir(dr)) != NULL)
        {
            struct stat s;
            stat(de->d_name, &s);
            fileType(s);
            // printf("%s\n",de->d_name);
            if (a_flag == 0) // to not show hidden files
            {
                if (de->d_name[0] != '.')
                {
                    if (l_flag == 0) // to not list details
                    {
                        printf("%s\n", de->d_name);
                    }
                    else // to list the details
                    {
                        // printf("%s\n", de->d_name);
                        printLine(s, de);
                    }
                }
            }
            else // to show hidden files
            {
                if (l_flag == 0) // to not list details
                {
                    printf("%s\n", de->d_name);
                }
                else // to list the details
                {
                    // printf("%s\n", de->d_name);
                    printLine(s, de);
                }
            }
        }
        printf("\033[0;37m");
        closedir(dr);
    }
    else if (counter == 1)
    {

        char cwd[256];
        getcwd(cwd, 256);
        struct dirent *de;
        if (strncmp(arguments[0], "~", 1) == 0)
        {
            char path[512];
            char *loginname = getlogin();
            int length = 6 + strlen(loginname);
            char cwd[256];
            getcwd(cwd, 256);
            int i = 0;
            for (; i < length; i++)
            {
                path[i] = cwd[i];
            }
            path[i] = '\0';
            chdir(path);
        }
        else if (strncmp(arguments[0], "-", 1) == 0)
        {
            chdir(warp->last);
        }
        else
        {
            chdir(arguments[0]);
        }
        DIR *dr = opendir(".");
        while ((de = readdir(dr)) != NULL)
        {
            struct stat s;
            stat(de->d_name, &s);
            fileType(s);
            // printf("%s\n",de->d_name);
            if (a_flag == 0) // to not show hidden files
            {
                if (de->d_name[0] != '.')
                {
                    if (l_flag == 0) // to not list details
                    {
                        printf("%s\n", de->d_name);
                    }
                    else // to list the details
                    {
                        // printf("%s\n", de->d_name);
                        printLine(s, de);
                    }
                }
            }
            else // to show hidden files
            {
                if (l_flag == 0) // to not list details
                {
                    printf("%s\n", de->d_name);
                }
                else // to list the details
                {
                    // printf("%s\n", de->d_name);
                    printLine(s, de);
                }
            }
        }
        printf("\033[0;37m");
        chdir(cwd);
        closedir(dr);
    }
    else
    {
        char cwd[256];
        getcwd(cwd, 256);
        for (int i = 0; i < counter; i++)
        {
            if (counter > 1)
            {
                printf("%s:\n", arguments[i]);
            }
            char cwd[256];
            getcwd(cwd, 256);
            struct dirent *de;
            chdir(arguments[i]);
            DIR *dr = opendir(".");
            while ((de = readdir(dr)) != NULL)
            {
                struct stat s;
                stat(de->d_name, &s);
                fileType(s);
                if (a_flag == 0) // to not show hidden files
                {
                    if (de->d_name[0] != '.')
                    {
                        if (l_flag == 0) // to not list details
                        {
                            printf("%s\n", de->d_name);
                        }
                        else // to list the details
                        {

                            printLine(s, de);
                        }
                    }
                }
                else // to show hidden files
                {
                    if (l_flag == 0) // to not list details
                    {
                        printf("%s\n", de->d_name);
                    }
                    else // to list the details
                    {
                        stat(de->d_name, &s);
                        printLine(s, de);
                    }
                }
                printf("\033[0;37m");
            }
            chdir("..");
            closedir(dr);
        }
        chdir(cwd);
    }
}
