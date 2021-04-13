#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "queue.h"

void InitQue(Que *que)
{
    if(que == NULL) exit(0);

    que->front = que->rear = NULL;
}

void Push(Que *que, DataType data)
{
    if(que == NULL) exit(0);

    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == NULL) return;

    new_node->data = data;
    new_node->next = NULL;

    if(que->front == NULL)
    {
        que->front = que->rear = new_node;
    }
    else
    {
        que->rear->next = new_node;
        que->rear = new_node;
    }   
}

int Empty(Que *que)
{
    if(que == NULL)  exit(0);
    return que->front == NULL;
}

DataType Pop(Que *que)
{
    if(que == NULL)  exit(0);

    if(Empty(que)) exit(0);

    DataType data = que->front->data;

    Node *p = que->front;

    que->front = que->front->next;

    free(p);

    if(que->front == NULL)  
    {
        que->rear = NULL;
    }

    return data;
}

void DestroyQue(Que *que)
{
    if(que == NULL) exit(0);

    while(!Empty(que))
    {
        Pop(que);
    }
}