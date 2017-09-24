#ifndef BUSYWAITMUTEX_STACK_H
#define BUSYWAITMUTEX_STACK_H

#include <stdbool.h>

typedef struct Node1 {
    int number;
    struct Node1* next;
} StackNode;

typedef struct Stack0 {
    StackNode* top;
    int length;
} Stack;

StackNode* malloc_stack_node(int number);

void free_stack_node(StackNode* node);

Stack* malloc_stack();

void free_stack(Stack* stack);

bool push(Stack* stack, int number);

int pop(Stack* stack);

bool is_empty(Stack* stack);

#endif //BUSYWAITMUTEX_STACK_H
