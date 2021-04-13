#pragma once

typedef int DataType;

typedef struct Node
{
    DataType data;
    struct Node *next;
}Node;

typedef struct Que
{
    Node *front;
    Node *rear;
}Que;

void InitQue(Que *que);

void Push(Que *que, DataType data);

int Empty(Que *que);

DataType Pop(Que *que);

void DestroyQue(Que *que);