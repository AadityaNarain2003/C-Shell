#include "neonate.h"
struct termios orig_termios_c;
void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

void disableRawMode_c()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_c);
}

void enableRawMode_c()
{
    tcgetattr(STDIN_FILENO, &orig_termios_c);
    atexit(disableRawMode_c);
    struct termios raw = orig_termios_c;

    // Disable flags for raw mode
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    // Disable software flow control
    raw.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Set character size to 8 bits
    raw.c_cflag |= (CS8);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    // Use a timeout of zero to check if input is available without blocking
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void neonate_karde(char *time_c)
{

    int time = atoi(time_c);
    if(time<=0)
    {
        printf("Sorry cannot be less than 1 second \n");
        return;
    }
    enableRawMode_c();
    while (1)
    {
        int last_pid = -1;
        DIR *dir;
        struct dirent *entry;

        dir = opendir("/proc");
        if (dir == NULL)
        {
            perror("opendir");
            exit(EXIT_FAILURE);
        }
        while ((entry = readdir(dir)))
        {
            // Check if the entry is a directory and its name is a number
            if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
            {
                int current_pid = atoi(entry->d_name);
                if (current_pid > last_pid)
                {
                    last_pid = current_pid;
                }
            }
        }

        closedir(dir);

        printf("%d\n", last_pid);
        // Check for keyboard input
        int i = 0;
        for (; i < time; i++)
        {
            delay(1000);

            if (kbhit() != 0)
            {
                char c;
                if (read(STDIN_FILENO, &c, 1) == 1)
                {
                    if (c == 'x')
                    {
                        printf("x encountered\n");
                        disableRawMode_c();
                        return; // Exit the loop when 'x' is pressed
                    }
                }
            }
        }
    }
}
// int main()
//{
//     neonate_karde("4");
// }