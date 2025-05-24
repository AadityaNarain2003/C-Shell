#ifndef _ACTIVITIES_H_
#define _ACTIVITIES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Head* ListPtr;
typedef struct Node* NodePtr;

struct Node
{
    int num;
    NodePtr next;
};

struct Head
{
    NodePtr top;
    int size;
};

ListPtr initList();
NodePtr initNode(int val);
void insertNode(ListPtr lp, int val);
void printActivities(ListPtr lp);

#endif