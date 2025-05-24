#include "headers.h"

int shell_pgid;
int curr_proc = -1;

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
    } // die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
    } // die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
    } // die("tcsetattr");
}

// With these modifications, your code will only attempt to enable or disable raw mode if standard input is associated with a terminal, which should help avoid the "Inappropriate ioctl for device" error when working with pipes.

void ctrl_c_handler(int sig)
{
    return;
}
void ctrl_z_handler(int sig)
{
    return;
}

int main()
{
    shell_pgid = getpgrp();
    //  to initialize structs and start my shell
    Home h = init_prompt();
    Storage s = init_Warp();
    Anish a = init_child();
    ListPtr lp = initList();

    signal(SIGINT, ctrl_c_handler);
    signal(SIGTSTP, ctrl_z_handler);

    char cwd[256];
    getcwd(cwd, 256);

    while (1)
    {

        // to check for background process if they are still running or not
        check_background(a);
        // to initialize pastebin
        PasteBin pb = file_initPasteBin(cwd);

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(h);

        // code to take input is here
        char *input = (char *)malloc(sizeof(char) * 10000);
        char *copy_input = (char *)malloc(sizeof(char) * 10000);

        setbuf(stdout, NULL);
        enableRawMode();
        // printf("Prompt>");
        memset(input, '\0', 10000);
        int pt = 0;
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (input[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        input[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                { // TAB character
                    input[pt++] = c;
                    for (int i = 0; i < 8; i++)
                    { // TABS should be 8 spaces
                        printf(" ");
                    }
                }
                else if (c == 4)
                { // ctrl D
                  // need to kill everything in Activities
                    printf("Sayonara!!!\n");
                    NodePtr temp = lp->top;
                    while (temp != NULL)
                    {
                        NodePtr temp2 = temp->next;
                        kill(temp->num, 9);
                        temp = temp2;
                    }

                    exit(0);
                }
                else
                {
                    printf("%d\n", c);
                }
            }
            else
            {
                input[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        printf("\n");
        if(strlen(input)==0)
        {
            printf("You cannot pass empty string!!! Sayonara!!!\n");
            exit(0);
        }
        strcpy(copy_input, input);
        // copy input will maintain a copy of the input that we will put into functios and update it
        // as we enter it into functions
        // printf("CS IS %s\n",copy_input);
        if (strncmp(copy_input, "exit", 4) == 0)
        {
            exit(0);
        }

        free(copy_input);
        copy_input = (char *)malloc(sizeof(char) * 10000);
        strcpy(copy_input, input);

        // to insert whatever i wrote into the past-event file
        insert(pb, copy_input);

        free(copy_input);
        copy_input = (char *)malloc(sizeof(char) * 10000);
        strcpy(copy_input, input);

        // NOW BEFORE ALL OF THIS WE NEED TO DO PIPING AND ALSO NEED TO STORE THE OUTPUT OF THE PREVIOUS FILE
        // IN A NEW FILE AND KEEP THE CHAIN GOING ON
        //  this will store the inputs from the argument line
            piping_maybe(input, copy_input, pb, s, h, a, lp);
    }
}
