#include <stdlib.h>
#include <stdio.h>
#include "sorted_list.h"

Node* malloc_node(int number) {
    Node* node;
    node = (Node*) malloc(sizeof(Node));

    if (node == NULL) {
        printf("Malloc error on malloc_node function!");
        return NULL;
    }

    node->number = number;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

void free_node(Node* node) {
    node->next = NULL;
    node->previous = NULL;
    free(node);
}

SortedList* malloc_sorted_list() {
    SortedList* list;
    list = (SortedList*) malloc(sizeof(SortedList));

    if (list == NULL) {
        printf("Malloc error on malloc_sorted_list function!");
        return NULL;
    }

    list->length = 0;
    list->first_node = NULL;
    list->last_node = NULL;

    return list;
}

void free_sorted_list(SortedList* list) {
    Node* node;
    node = list->first_node;

    if (node != NULL) {
        while (node->next != NULL) {
            node = node->next;
            free(node->previous);
        }

        free(node);
    }

    free(list);
}

bool add_first(SortedList* list, Node* node) {
    if (list->length == 0)
        return false;

    list->first_node->previous = node;
    node->next = list->first_node;
    list->first_node = node;

    list->length++;

    return true;
}

bool add_last(SortedList* list, Node* node) {
    if (list->length == 0)
        return false;

    list->last_node->next = node;
    node->previous = list->last_node;
    list->last_node = node;

    list->length++;

    return true;
}

bool add_number(SortedList* list, int number) {
    Node* node;
    node = malloc_node(number);

    if (list->length == 0) {
        list->first_node = node;
        list->last_node = node;
    } else if (number <= list->first_node->number)
        return add_first(list, node);
    else if (number >= list->last_node->number)
        return add_last(list, node);
    else {
        Node* previous;
        previous = list->first_node;
        Node* current;
        current = previous->next;
        Node* next;
        next = current->next;

        while (current != list->last_node) {
            if (number <= current->number) {    // If it's smaller than the current
                previous->next = node;
                node->previous = previous;
                node->next = current;
                current->previous = node;
                break;
            } else if (next == list->last_node) {   // If it's only smaller than the last
                current->next = node;
                node->previous = current;
                node->next = next;
                next->previous = node;
                break;
            }

            previous = current;
            current = next;
            next = next->next;
        }
    }

    list->length++;

    return true;
}

void print_sorted_list(SortedList* list) {
    Node* node;
    node = list->first_node;

    printf("[ ");

    while (node != NULL) {
        printf("%d", node->number);

        if (node->next != NULL)
            printf(", ");

        node = node->next;
    }

    printf(" ]\n");
}
