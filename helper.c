#include "helper.h"

// Function to trim leading and trailing spaces from a string
char *trimSpaces(char *str)
{
    if (str == NULL)
        return NULL;

    int len = strlen(str);
    int start = 0;     // Index for the start of the trimmed string
    int end = len - 1; // Index for the end of the trimmed string

    // Trim leading spaces
    while (isspace(str[start]))
    {
        start++;
    }

    // If the entire string is spaces, return an empty string
    if (start == len)
    {
        str[0] = '\0';
        return str;
    }

    // Trim trailing spaces
    while (isspace(str[end]))
    {
        end--;
    }

    // Copy the trimmed portion of the string to the beginning
    int trimmedLen = end - start + 1;
    memmove(str, str + start, trimmedLen);
    str[trimmedLen] = '\0';

    return str;
}

Anish init_child()
{
    Anish a = (Anish)malloc(sizeof(struct Child));
    a->pids = (int *)malloc(sizeof(int) * 100);
    a->names = (char **)malloc(sizeof(char *) * 100);
    a->validity = (int *)calloc(sizeof(int), 100);
    a->curr = 0;
    return a;
}

void tokenizing(char *input, char **arguments, int *counter, const char *s)
{
    // s is the delimeter that we tokenize on
    char *token = strtok(input, s);
    while (token != NULL)
    {
        arguments[*counter] = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(arguments[*counter], token);

        token = strtok(NULL, s);
        *counter = 1 + *counter;
    }
}
int cmp_string(char *a)
{
    int len = strlen(a);
    for (int i = 0; i < len; i++)
    {
        if (a[i] == '&')
        {
            return 1;
        }
    }
    return 0;
}
// needs to run if backgrond processes are there
void run_foreground_background(char *copy_input, Anish a, ListPtr lp)
{
    char **arguments2 = (char **)malloc(sizeof(char *) * MAX_ARGS);
    int counter2 = 0;
    tokenizing(copy_input, arguments2, &counter2, "&");
    //["sleep 5" , "sleep 2", " "]
    // arguments2[counter2 - 1][strlen(arguments2[counter2 - 1]) - 1] = '\0';

    for (int j = 0; j < counter2 - 1; j++)
    {

        int f = fork();
        if (f == 0) // it is child
        {
            char **arguments3 = (char **)malloc(sizeof(char *) * MAX_ARGS);
            int counter3 = 0;
            tokenizing(arguments2[j], arguments3, &counter3, " ");
            arguments3[counter3] = malloc(sizeof(char) * 5);
            arguments3[counter3++] = NULL;
            if (execvp(arguments3[0], arguments3) != 0)
            {
                perror("Failure!!!\n");
                exit(EXIT_FAILURE);
            }
            // printf("in child\n");
        }
        else // it is parent
        {
        }
        insertNode(lp, f);
        a->pids[a->curr] = f;
        a->names[a->curr] = (char *)malloc(sizeof(char) * 100);
        a->validity[a->curr] = 1;
        strcpy(a->names[a->curr++], arguments2[j]);
        printf("[%d]    \t%d\n", j + 1, f);
    }

    // this is to execute the last statement
    int g = fork();
    if (g == 0)
    {
        char **arguments4 = (char **)malloc(sizeof(char *) * MAX_ARGS);
        int counter4 = 0;
        tokenizing(arguments2[counter2 - 1], arguments4, &counter4, " ");
        arguments4[counter4] = malloc(sizeof(char) * 5);
        arguments4[counter4++] = NULL;
        if (execvp(arguments4[0], arguments4) != 0)
        {
            perror("Failure!!!\n");
            exit(EXIT_FAILURE);
        }
    }
    setpgid(g, 0);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, g);

    int t;
    waitpid(g, &t, WUNTRACED);
    // Set parent back to foreground process gid
    tcsetpgrp(STDIN_FILENO, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    if (WIFSTOPPED(t))
    {
        insertNode(lp, g);
    }
}
// this function only runs execvp commands
void run_other_commands(char *copy_input, ListPtr lp)
{

    // copy_input[strlen(copy_input) - 1] = '\0';printf("|%s|\n",copy_input);
    char **arguments3 = (char **)malloc(sizeof(char *) * MAX_ARGS);
    int counter3 = 0;
    tokenizing(copy_input, arguments3, &counter3, " ");
    arguments3[counter3] = malloc(sizeof(char) * 5);
    arguments3[counter3++] = NULL;
    int f = fork();
    if (f == 0)
    {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (execvp(arguments3[0], arguments3) == -1)
        {
            perror("error executing command!!!\n");
            exit(EXIT_FAILURE);
        }
    }
    setpgid(f, 0);

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, f);

    int t;
    waitpid(f, &t, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    if (WIFSTOPPED(t))
    {
        insertNode(lp, f);
    }
}
// this function basically runs all commands , excevp and self made
void run_commands(char *arguments, PasteBin pb, Storage s, char *copy_input, Home h, ListPtr lp)
{

    char tok[4] = " ";
    char *new_token = strtok(arguments, tok);

    if (strncmp(new_token, "pastevents", 10) == 0)
    {
        new_token = strtok(NULL, "\0");
        pastevent_caller(pb, new_token);
    }
    else if (strncmp(new_token, "warp", 4) == 0)
    {
        new_token = strtok(NULL, "\0");
        warp(s, new_token);
    }
    else if (strncmp(new_token, "peek", 4) == 0)
    {
        new_token = strtok(NULL, "\0");
        peek(s, new_token);
    }
    else if (strncmp(new_token, "proclore", 8) == 0)
    {
        new_token = strtok(NULL, "\0");
        proc(new_token);
    }
    else if (strncmp(new_token, "seek", 4) == 0)
    {
        new_token = strtok(NULL, "\0");
        seeking1(new_token, h->login);
    }
    else if (strncmp(new_token, "ping", 4) == 0)
    {
        new_token = strtok(NULL, "\0");
        sendsig_util(new_token);
    }
    else if (strncmp(new_token, "activities", 10) == 0)
    {
        printActivities(lp);
    }
    else if (strncmp(new_token, "iman", 4) == 0)
    {
        new_token = strtok(NULL, "\0");
        imanwow(new_token);
    }
    else if (strncmp(new_token, "fg", 2) == 0)
    {
        new_token = strtok(NULL, "\0");
        fg_try(new_token);
    }
    else if (strncmp(new_token, "bg", 2) == 0)
    {
        new_token = strtok(NULL, "\0");
        bg_try(new_token);
    }
    else if (strncmp(new_token, "neonate", 7) == 0)
    {
        new_token = strtok(NULL, "\0");
        //printf("%s\n",new_token);
        int start=0;
        for(int i=0;i<strlen(new_token)-2;i++)
        {
            if(new_token[i]=='-' && new_token[i+1]=='n')
            {
                start=i+2;
                break;
            }
        }
        char* new_num=malloc(sizeof(char)*10);
        strcpy(new_num,new_token);
        for(int i=0;i<strlen(new_num);i++)
        {
            new_num[i]=new_num[i+3];

        }
        //printf("|%s|\n",new_num);
        neonate_karde(new_num);
    }
    else
    {
        run_other_commands(copy_input, lp);
    }
}
void file_checkers(char *arguments, PasteBin pb, Storage s, char *copy_input, Home h, ListPtr lp)
{
    Files f = init_file_systems();
    check_flag_input(f, arguments);
    check_flag_append(f, arguments);
    check_flag_output(f, arguments);
    if (f->flag_append == -1 && f->flag_input == -1 && f->flag_output == -1)
    {
        run_commands(arguments, pb, s, copy_input, h, lp);
    }
    else
    {
        char *input_file = NULL;
        char *output_file = NULL;
        char *append_file = NULL;
        const char *token = ">><>";
        int counter = 0;
        char **argument = (char **)malloc(sizeof(char *) * MAX_ARGS);
        arguments[strlen(arguments) ] = '\0';
        tokenizing(arguments, argument, &counter, token);
        // the string has been tokenized to be formatted according to the >, < and >> symbols
        // the next part of code tries to find the names of input ,output and append files
        if (f->flag_input >= 0) // to get the input string
        {
            input_file = (char *)malloc(sizeof(char) * MAX_ARGS);
            int i = 0;
            int pos = 1;
            while (i == 0 || arguments[f->flag_input + pos] != '\0')
            {
                if (arguments[f->flag_input + pos] == '\0')
                {
                    pos++;
                }
                else
                {
                    input_file[i++] = arguments[f->flag_input + (pos++)];
                }
            }
            input_file[pos - 1] = '\0';
        }
        if (f->flag_output >= 0) // to get the output string
        {
            output_file = (char *)malloc(sizeof(char) * MAX_ARGS);
            int i = 0;
            int pos = 1;
            while (i == 0 || arguments[f->flag_output + pos] != '\0')
            {
                if (arguments[f->flag_output + pos] == '\0')
                {
                    pos++;
                }
                else
                {
                    output_file[i++] = arguments[f->flag_output + (pos++)];
                }
            }
            output_file[pos - 1] = '\0';
        }
        if (f->flag_append >= 0) // to get the append string
        {
            append_file = (char *)malloc(sizeof(char) * MAX_ARGS);
            int i = 0;
            int pos = 1;
            while (i == 0 || arguments[f->flag_append + pos] != '\0')
            {
                if (arguments[f->flag_append + pos] == '\0')
                {
                    pos++;
                }
                else
                {
                    append_file[i++] = arguments[f->flag_append + (pos++)];
                }
            }
            append_file[pos - 1] = '\0';
            int j = 1;
            for (; j < strlen(append_file); j++)
            {
                append_file[j - 1] = append_file[j];
            }
            append_file[j - 1] = '\0';
        }
        char *out_file = NULL;
        char out_mode[1];
        if (f->flag_append >= 0 || f->flag_output >= 0)
        {
            // as we can just output to one of them therefore we will use just one of them
            // use the one appearing first
            out_file = (char *)malloc(sizeof(char) * MAX_ARGS);
            if (f->flag_append <= f->flag_output && f->flag_append >= 0)
            {
                out_mode[0] = 'a';
                strcpy(out_file, append_file);
            }
            else if (f->flag_output >= 0)
            {
                out_mode[0] = 'w';
                strcpy(out_file, output_file);
            }
            else
            {
                out_mode[0] = 'a';
                strcpy(out_file, append_file);
            }
        }
        // input_file contains if there is a file to be inputed
        // out_file contains if there is a file to be outputed
        // out_mode contains the mode of the output file
        // assuming that only 1 file can be written to

        // now i need to trim the spaces in both the input and out file if they exist
        int original_stdout = dup(STDOUT_FILENO);
        int original_stdin = dup(STDIN_FILENO);
        {
            if (input_file != NULL)
            { // if '<' char was found in string inputted by user
                input_file = trimSpaces(input_file);
                int fd0 = open(input_file, O_RDONLY, 0);
                dup2(fd0, STDIN_FILENO);
                close(fd0);
            }

            if (out_file != NULL)
            { // if '>' was found in string inputted by user
                int fd;
                out_file = trimSpaces(out_file);
                if (out_mode[0] == 'w')
                {
                    fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                else
                {
                    fd = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            // now i think i need to send only some part of the files
            // printf("|%s|\n",argument[0]);
            // run_commands(argument[0],pb,s,argument[0],h);
            run_other_commands(argument[0], lp);
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
            dup2(original_stdin, STDIN_FILENO);
            close(original_stdin);
        }
    }
    free(f);
}
// function to check if background process is done executing
void check_background(Anish a)
{
    for (int i = 0; i < a->curr; i++)
    {
        int status;
        int done = waitpid(a->pids[i], &status, WNOHANG);
        if (done && a->validity[i] == 1)
        {
            printf("exited normally     %s\n", a->names[i]);
            a->validity[i] = 0;
        }
    }
}
// function to run whatever we have written and is seperated by ;
void run_all(char *input, PasteBin pb, Storage s, Home h, Anish a, ListPtr lp)
{
    // printf("IN RUN\n");
    char **arguments = (char **)malloc(sizeof(char *) * MAX_ARGS);
    int counter = 0;
    tokenizing(input, arguments, &counter, ";");

    //  now executing each command
    for (int i = 0; i < counter; i++)
    {
        char *copy_input = (char *)malloc(sizeof(char) * 10000);
        strcpy(copy_input, arguments[i]);
        // to divide based on & if it exists
        if (cmp_string(copy_input) == 1)
        {
            run_foreground_background(copy_input, a, lp);
        }
        // now that the commands have been seperated by ; we can easily execute them
        else
        {
            {
                file_checkers(arguments[i], pb, s, copy_input, h, lp);
            }
        }
    }
}

void piping_maybe(char *input, char *copy_input, PasteBin pb, Storage s, Home h, Anish a, ListPtr lp)
{
    char **commands = (char **)malloc(sizeof(char *) * MAX_ARGS);
    int counter_commands = 0;
    tokenizing(input, commands, &counter_commands, "|");

    int p[2];
    pid_t pid;
    int fd_in = 0;
    if (counter_commands == 1)
    {

        {
            run_all(commands[0], pb, s, h, a, lp);
        }

    }
    else
    {
        for (int i = 0; i < counter_commands; i++)
        {
            pipe(p);
            if ((pid = fork()) == -1)
            {
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                dup2(fd_in, 0); // change the input according to the old one

                dup2(p[1], 1);
                close(p[0]);
                run_all(commands[i], pb, s, h, a, lp);
                exit(EXIT_FAILURE);
            }
            else
            {
                waitpid(pid, NULL, 0);
                close(p[1]);
                fd_in = p[0]; // save the input for the next command
            }
        }
        char temp_var;
        while (read(fd_in, &temp_var, 1))
        {
            printf("%c", temp_var);
        }
    }
    file_writePasteBin(pb);
}