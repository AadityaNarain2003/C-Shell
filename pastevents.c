#include "pastevents.h"

char FILE_NAME[400] ;

void int_to_str(int num, char *str, int size) // size of int is 2
{
    int pos = size-1;
    while (num > 0)
    {
        int rem = num % 10;
        str[pos] = (char)(rem + 48);
        num = num / 10;
        pos--;
    }
}

void print_BIN(PasteBin p)
{
    printf("Size: %d  Position: %d\n", p->curr_size, p->curr_pos);
    for (int i = 0; i < p->curr_size; i++)
    {
        printf("%d.%s\n", i, p->commands[i]);
    }
}

PasteBin initPasteBin(char * cwd)
{
    PasteBin pb = (PasteBin)malloc(sizeof(struct Paste));
    pb->curr_pos = 0;
    pb->curr_size = 0;
    pb->commands = (char **)malloc(sizeof(char *) * SIZE);
    
    for (int i = 0; i < SIZE; i++)
    {
        // this place is giving malloc(); invalid size (unsorted)
        // need to check it later
        // printf("%d ", i);
        pb->commands[i] = (char *)malloc(sizeof(char) * MAX_SIZE);
    }
    sprintf(FILE_NAME,"%s/bin.txt",cwd);
    return pb;
}
PasteBin file_initPasteBin(char * cwd)
{
    PasteBin pb = initPasteBin(cwd);
    
    long long int input_file = open(FILE_NAME, O_RDONLY | O_CREAT, 0777);

    struct stat s;
    stat(FILE_NAME, &s);
    int size = s.st_size;
    char *str = (char *)calloc(sizeof(char), 16001);
    if (size != 0)
    {

        int r = read(input_file, str, 16001);
        // need to divide it by strtok and use it
        const char s[4] = "\n";
        char *token = strtok(str, s);
        pb->curr_size = atoi(token);
        token = strtok(NULL, s);
        pb->curr_pos = atoi(token);
        token = strtok(NULL, s);
        int i = 0;
        while (token != NULL && i < SIZE)
        {
            strcpy(pb->commands[i], token);
            i++;
            token = strtok(NULL, s);
        }
    }
    free(str);
    // need to edit this accordingly and write in the files
    return pb;
}

void file_writePasteBin(PasteBin pb)
{
    //printf("here %d %d\n",pb->curr_pos,pb->curr_size);
    remove(FILE_NAME);
    long long int input_file = open(FILE_NAME, O_RDWR | O_CREAT, 0777);
    char number_s[3];
    sprintf(number_s, "%2d", pb->curr_size);
    write(input_file, number_s, 2);
    write(input_file, "\n", 1);

    sprintf(number_s, "%2d", pb->curr_pos);
    write(input_file, number_s, 2);
    write(input_file, "\n", 1);

    for (int i = 0; i < pb->curr_size; i++)
    {
        write(input_file, pb->commands[i], strlen(pb->commands[i]));
        write(input_file, "\n", 1);
    }
    for (int i = 0; i < pb->curr_size; i++)
    {
        free(pb->commands[i]);
    }
    // free(pb->commands);
    // pb->curr_pos=0;
    // pb->curr_size=0;
    free(pb);
}

int insert(PasteBin pb, char *txt)
{
    char *temp = (char *)malloc(sizeof(char) * 4096);
    int pos = 0;
    const char s[4] = " ";
    char *token = strtok(txt, s);
    // this part is to create the temp string without the extra spaces and stuff

    while (token != NULL)
    {

        // now to copy elements from token to temp
        int i = 0;
        for (; i < strlen(token); i++)
        {
            temp[pos + i] = token[i];
        }
        temp[pos + i] = ' ';
        pos = pos + i + 1;
        token = strtok(NULL, s);
    }
    temp[pos ] = '\0';
    pos = strlen(temp) - 1;
    /* while(pos>=0 && temp[pos]==' ' )
     {

         pos--;
     }
     temp[pos+1]='\0';*/
    // printf("%s\n",pb->commands[((pb->curr_pos)-2)%SIZE]);
    // printf("%s\n",temp);
    int t = (pb->curr_pos - 1);
    if (t < 0)
    {
        t = t + 15;
    }
    int a = strcmp(pb->commands[t % SIZE], temp);
    if (pb->curr_size > 0 && (a == 22 || a == -10 || a==0))
    {
        return 1;
        // do nothing as it is the same as the last command
    }
    else
    {
        // this is to copy the elements into the next position and overwrite the oldest data if there
        if (pb->curr_size == 0)
        {
            free(pb->commands[(pb->curr_pos) % SIZE]);
            pb->commands[(pb->curr_pos) % SIZE] = (char *)malloc(sizeof(char) * MAX_SIZE);
            strcpy(pb->commands[(pb->curr_pos) % SIZE], temp);
            pb->curr_pos = (pb->curr_pos + 1) % SIZE;
        }
        else
        {
            temp[strlen(temp) - 1] = ' ';
            free(pb->commands[(pb->curr_pos) % SIZE]);
            pb->commands[(pb->curr_pos) % SIZE] = (char *)malloc(sizeof(char) * MAX_SIZE);
            strcpy(pb->commands[(pb->curr_pos) % SIZE], temp);
            pb->curr_pos = (pb->curr_pos + 1) % SIZE;
        }
        if (pb->curr_size < SIZE)
        {
            pb->curr_size++;
        }
    }
    return 1;
}
void purge(PasteBin pb)
{
    remove(FILE_NAME);
    for (int i = 0; i < pb->curr_size; i++)
    {
        free(pb->commands[i]);
    }
    free(pb->commands);
    pb->curr_pos = 0;
    pb->curr_size = 0;
}

char *execute(PasteBin pb, int index)
{
    if (pb->curr_size == 0)
    {
        return "";
    }
    if (index > pb->curr_size)
    {
        // this means the command doesnot exist, therefore return an error
        return NULL;
    }
    int temp = pb->curr_pos-1;
    if(temp<0)
    {
        temp=pb->curr_size+temp;
    }
    while (index > 0)
    {
        temp--;
        if (temp < 0)
        {
            temp = SIZE - 1;
        }
        index--;
    }
    return pb->commands[temp];
}
void printAll(PasteBin pb)
{
    if (pb->curr_size == 0)
    {
        return;
    }
    int temp = pb->curr_pos;
    for (int i = 0; i < pb->curr_size; i++)
    {
        printf("%s\n", pb->commands[temp % pb->curr_size]);
        temp++;
        temp = temp % pb->curr_size;
    }
}

void pastevent_caller(PasteBin pb, char *input)
{
    // printf("IN PASTEVENT:\nINPUT IS:    %s\n",input);
    char s[4] = " ";
    char *token = strtok(input, s);
    if (input == NULL || token == NULL || input[0]==' ')
    {
        printAll(pb);
        return;
    }
    else
    {
        if (strncmp(token, "purge", 5) == 0)
        {
            purge(pb);
            return;
        }
        if (strncmp(token, "execute", 7) == 0)
        {
            token = strtok(NULL, s);
            int x = atoi(token);
            char *a = execute(pb, x);
            printf("%s\n",a);
            return;
        }
    }
}
