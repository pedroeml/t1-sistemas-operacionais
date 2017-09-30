#ifndef BUSYWAITMUTEX_SORTED_LIST_H
#define BUSYWAITMUTEX_SORTED_LIST_H

#include <stdbool.h>

typedef struct Node0 {
    int number;
    struct Node0* next;
    struct Node0* previous;
} Node;

typedef struct List0 {
    Node* first_node;
    Node* last_node;
    int length;
} SortedList;

Node* malloc_node(int number);

void free_node(Node* node);

SortedList* malloc_sorted_list();

void free_sorted_list(SortedList* list);

bool add_number(SortedList *list, int number);

void print_sorted_list(SortedList* list);

#endif //BUSYWAITMUTEX_SORTED_LIST_H
