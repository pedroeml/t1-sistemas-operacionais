#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "mutex.h"
#include "sorted_list.h"

#define NUM_PRODUCERS 4

Mutex* mutex;
SortedList* list;

bool is_even(int number) {
    return number % 2 == 0;
}

bool is_prime(int number) {
    if (number == 1 || number == 2)
        return true;
    else if (is_even(number))
        return false;
    /*
     * Considering it's not an even number, try every possible
     * odd divisor within its half.
     */
    for (int divisor = 3; divisor <= number/2; divisor += 2) {
        if (number % divisor == 0)
            return false;
    }

    return true;    // If this number has no divisor in that range, then it's prime.
}

void test_numbers(int start, int end, int thread_number) {
    for (int number = start; number <= end; number += 2) {
        if (is_prime(number)) {
            printf("Thread %d found %d as prime number.\n", thread_number, number);
            lock(mutex, thread_number);
            // TODO: Create a Consumer Thread to add prime numbers to the list
            add_number(list, number);
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

    printf("Thread %d finished\n", thread_number);

    free(params);

    pthread_exit(NULL);

    return NULL;
}


int** create_ranges(int interval) {
    int** ranges;
    ranges = (int**) malloc(sizeof(int*)*NUM_PRODUCERS);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* row;
        row = (int*) malloc(sizeof(int)*2);

        *row = 1 + interval*i;
        *(row+1) = interval + interval*i;

        *(ranges+i) = row;
    }

    return ranges;
}

void print_arr(int** arr) {
    printf("[ ");

    int* row;
    for (int i = 0; i < NUM_PRODUCERS - 1; i++) {
        row = *(arr + i);
        printf(" [%d, %d]", *row, *(row + 1));
    }

    row = *(arr + NUM_PRODUCERS - 1);
    printf(" [%d, %d] ]\n", *row, *(row + 1));
}

int* generate_params(int start, int end, int thread_number) {
    int* params;
    params = (int*) malloc(sizeof(int)*3);

    *params = start;
    *(params+1) = end;
    *(params+2) = thread_number;

    return params;
}

int main() {
    int** ranges;
    ranges = create_ranges(1000);
    print_arr(ranges);

    pthread_t threads[NUM_PRODUCERS];
    int err_code;
    int* row;
    int* params;

    mutex = malloc_mutex(NUM_PRODUCERS);
    list = malloc_sorted_list();

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        row = *(ranges + i);
        params = generate_params(*row, *(row+1), i);

        err_code = pthread_create(&threads[i], NULL, find_primes, (void*) params);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(threads[i], NULL);
    }

    print_sorted_list(list);
    free_sorted_list(list);

    free_mutex(mutex);

    printf("Thread main finished\n");
    pthread_exit(NULL);

    return 0;
}
