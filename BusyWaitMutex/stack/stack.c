#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

StackNode* malloc_stack_node(int number) {
    StackNode* node;
    node = (StackNode*) malloc(sizeof(StackNode));

    if (node == NULL) {
        printf("Malloc error on malloc_stack_node function!");
        return NULL;
    }

    node->number = number;
    node->next = NULL;

    return node;
}

void free_stack_node(StackNode* node) {
    node->next = NULL;
    free(node);
}

Stack* malloc_stack() {
    Stack* stack;
    stack = (Stack*) malloc(sizeof(Stack));

    if (stack == NULL) {
        printf("Malloc error on malloc_stack function!");
        return NULL;
    }

    stack->length = 0;
    stack->top = NULL;

    return stack;
}

void free_stack(Stack* stack) {
    StackNode* node;
    node = stack->top;

    if (node != NULL) {
        StackNode* previous;
        previous = node;
        node = node->next;

        while (node != NULL) {
            free(previous);
            previous = node;
            node = node->next;
        }

        free(previous);
    }

    free(stack);
}

bool push(Stack* stack, int number) {
    StackNode* node;
    node = malloc_stack_node(number);

    StackNode* previous_top;
    previous_top = stack->top;

    stack->top = node;
    node->next = previous_top;

    stack->length++;

    return true;
}

int pop(Stack* stack) {
    if (stack->length == 0) {
        printf("Poping error: empty stack!\n");
        return NULL;
    }

    StackNode* node;
    node = stack->top;

    stack->top = node->next;
    stack->length--;

    int number = node->number;

    free(node);

    return number;
}

bool is_empty(Stack* stack) {
    return stack->length == 0;
}
