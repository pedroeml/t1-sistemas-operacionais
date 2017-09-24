#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex/lamports_bakery.h"
#include "list/sorted_list.h"
#include "stack/stack.h"
#include "utils.h"

#define NUM_PRODUCERS 4

Mutex* mutex;
Stack* stack;

void* consumer(void *param) {
    bool* producers_stopped;
    producers_stopped = param;

    SortedList* list;
    list = malloc_sorted_list();

    while(!*producers_stopped || !is_empty(stack))    // While the producers still producing or stack isn't empty
        if (!is_empty(stack))     // Makesure it isn't empty
            add_number(list, pop(stack));

    printf("Producers Threads have finished\n");

    print_sorted_list(list);
    free_sorted_list(list);

    printf("Consumer Thread has finished\n");

    pthread_exit(NULL);

    return NULL;
}

void test_numbers(int start, int end, int thread_number) {
    for (int number = start; number <= end; number += 2) {
        if (is_prime(number)) {
            printf("Producer Thread %d found %d as prime number.\n", thread_number, number);
            lock(mutex, thread_number);
            push(stack, number);
            unlock(mutex, thread_number);
        }
    }
}

void* find_primes(void* params) {
    int* parameters = params;
    int start = *parameters;
    int end = *(parameters + 1);
    int thread_number = *(parameters + 2);

    printf("Thread number: %d\tRange: [%d:%d]\n", thread_number, start, end);

    if (is_even(start)) {
        if (is_even(end)) {
            test_numbers(start + 1, end - 1, thread_number);
        } else {
            test_numbers(start + 1, end, thread_number);
        }
    } else {
        if (is_even(end)) {
            test_numbers(start, end - 1, thread_number);
        } else {
            test_numbers(start, end, thread_number);
        }
    }

    printf("Producer Thread %d has finished\n", thread_number);

    free(params);

    pthread_exit(NULL);

    return NULL;
}

int producer_consumer() {
    int** ranges;
    ranges = create_ranges(1000, NUM_PRODUCERS);
    print_arr(ranges, NUM_PRODUCERS);

    pthread_t threads[NUM_PRODUCERS];
    int err_code;
    int* row;
    int* params;
    bool producers_stopped = false;

    mutex = malloc_mutex(NUM_PRODUCERS);
    stack = malloc_stack();

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        row = *(ranges + i);
        params = generate_params(*row, *(row+1), i);

        err_code = pthread_create(&threads[i], NULL, find_primes, (void*) params);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    pthread_t consumer_thread;
    err_code = pthread_create(&consumer_thread, NULL, consumer, (void*) &producers_stopped);

    if (err_code){
        printf("ERROR code is %d\n", err_code);
        return -1;
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(threads[i], NULL);
    }

    producers_stopped = true;

    pthread_join(consumer_thread, NULL);

    free_stack(stack);
    free_mutex(mutex);

    printf("Thread main finished\n");
    pthread_exit(NULL);

    return 0;
}

int readers_writers() {
    // TODO: Implementation

    return 0;
}

int main() {
    // TODO: Implement something to the user select which problem he wants to run
    return producer_consumer();
}
