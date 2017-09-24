#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex/lamports_bakery.h"
#include "list/sorted_list.h"
#include "stack/stack.h"
#include "utils.h"

#define NUM_THREADS 5
#define RANGE 1000

Mutex* mutex;
Stack* stack;
const int CONSUMER_THREAD_INDEX = NUM_THREADS - 1;

void* consume(void *param) {
    bool* producers_stopped;
    producers_stopped = param;

    SortedList* list;
    list = malloc_sorted_list();

    while(!*producers_stopped || !is_empty(stack))    // While the producers still producing or stack isn't empty
        if (!is_empty(stack)) {     // Makesure it isn't empty
            lock(mutex, CONSUMER_THREAD_INDEX);
            int number = pop(stack);
            unlock(mutex, CONSUMER_THREAD_INDEX);
            add_number(list, number);
        }

    printf("Producers Threads have finished\n");

    print_sorted_list(list);
    printf("List's length: %d\n", list->length);
    free_sorted_list(list);

    printf("Consumer Thread has finished\n");

    pthread_exit(NULL);

    return NULL;
}

void find_prime_numbers(int start, int end, int thread_number) {
    for (int number = start; number <= end; number += 2) {
        if (is_prime(number)) {
            printf("Producer Thread %d found %d as prime number.\n", thread_number, number);
            lock(mutex, thread_number);
            push(stack, number);
            unlock(mutex, thread_number);
        }
    }
}

void* produce(void *params) {
    int* parameters = params;
    int start = *parameters;
    int end = *(parameters + 1);
    int thread_number = *(parameters + 2);

    printf("Thread number: %d\tRange: [%d:%d]\n", thread_number, start, end);

    if (is_even(start)) {
        if (is_even(end)) {
            find_prime_numbers(start + 1, end - 1, thread_number);
        } else {
            find_prime_numbers(start + 1, end, thread_number);
        }
    } else {
        if (is_even(end)) {
            find_prime_numbers(start, end - 1, thread_number);
        } else {
            find_prime_numbers(start, end, thread_number);
        }
    }

    printf("Producer Thread %d has finished\n", thread_number);

    free(params);

    pthread_exit(NULL);

    return NULL;
}

int producer_consumer() {
    int** ranges;
    ranges = create_ranges(RANGE, CONSUMER_THREAD_INDEX);
    print_arr(ranges, CONSUMER_THREAD_INDEX);

    pthread_t threads[NUM_THREADS];
    int err_code;
    int* row;
    int* params;
    bool producers_stopped = false;

    mutex = malloc_mutex(NUM_THREADS);
    stack = malloc_stack();

    // Creating the producers threads
    for (int i = 0; i < CONSUMER_THREAD_INDEX; i++) {   // The last index is for the producer
        row = *(ranges + i);
        params = generate_params(*row, *(row+1), i);

        err_code = pthread_create(&threads[i], NULL, produce, (void *) params);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    // Creating the consumer thread
    err_code = pthread_create(&threads[CONSUMER_THREAD_INDEX], NULL, consume, (void*) &producers_stopped);

    if (err_code){
        printf("ERROR code is %d\n", err_code);
        return -1;
    }

    for (int i = 0; i < CONSUMER_THREAD_INDEX; i++) {
        pthread_join(threads[i], NULL);
    }

    producers_stopped = true;

    pthread_join(threads[CONSUMER_THREAD_INDEX], NULL);

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
