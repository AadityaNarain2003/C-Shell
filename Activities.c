#include "Activities.h"

NodePtr initNode(int val)
{
    NodePtr np = (NodePtr)malloc(sizeof(struct Node));
    np->num = val;
    np->next = NULL;
    return np;
}

ListPtr initList()
{
    ListPtr lp = (ListPtr)malloc(sizeof(struct Head));
    lp->top = NULL;
    lp->size = 0;
    return lp;
}

void insertNode(ListPtr lp, int val)
{

    if (lp == NULL)
    {

        return;
    }
    if (lp->size == 0)
    {
        lp->top = initNode(val);
        lp->size = 1;
    }
    else
    {
        NodePtr temp = initNode(val);
        temp->next = lp->top;
        lp->top = temp;
        lp->size = lp->size + 1;
    }
}
void printActivities(ListPtr lp)
{
    NodePtr np = lp->top;
    while (np != NULL)
    {
        char path[1024];
        sprintf(path, "/proc/%d/status", np->num);
        FILE *fi = fopen(path, "r");
        if (fi == NULL)
        {
            // printf("PROCESS doesnt exist\n");
            // return;
        }
        else
        {
            char buffer[1024];
            char name[20], state[20];
            while (fgets(buffer, 1024, fi))
            {
                // printf("%s",buffer);
                if (strncmp(buffer, "State:", 6) == 0)
                {
                    sscanf(buffer + 7, "%s", state);
                }
                if (strncmp(buffer, "Name:", 5) == 0)
                {
                    sscanf(buffer + 6, "%s", name);
                }
            }
            const char *stateStr = NULL;
            if (strncmp(state, "R", 1) == 0)
            {
                stateStr = "running";
            }
            else if (strncmp(state, "S", 1) == 0 || strncmp(state, "Z", 1) == 0 || strncmp(state, "T", 1) == 0)
            {
                stateStr = "stopped";
            }
            else
            {
                stateStr = "unknown"; // Handle other states if needed
            }

            {
                printf("%d : %s - %s\n", np->num, name, stateStr);
            }
        }
        // printf("--------------------\n");
        np = np->next;
    }
}
