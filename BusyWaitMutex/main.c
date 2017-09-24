#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "mutex.h"

#define NUM_THREADS 4

Mutex* mutex;

/*
 * Problema 1:  Suponha que sejam criados 5 threads. Utilize
 * semáforos para garantir que a thread 1 escreva números de
 * 1 até 200, a thread 2 os números de 201 até 400, e assim
 * por diante.
 * Dica: a thread i+1 só deve entrar em funcionamento quando
 * a thread i já tiver terminado a escrita dos seus números.
 */

void* print_numbers_in_range(void* params) {
    int* parameters = params;
    int start = *parameters;
    int end = *(parameters + 1);
    int thread_number = *(parameters + 2);

    lock(mutex, thread_number);
    printf("Thread number: %d\tRange: [%d:%d]\n", thread_number, start, end);

    for (int i = start; i <= end; i++)
        printf("%d\n", i);

    printf("Thread %d finished\n", thread_number);
    unlock(mutex, thread_number);

    free(params);

    pthread_exit(NULL);

    return NULL;
}

int** create_ranges(int interval) {
    int** ranges;
    ranges = (int**) malloc(sizeof(int*)*NUM_THREADS);

    /*
     * The for loop below does the following:
     *  interval = 200
     *  1 + 200*0 = 1       200 + 200*0 = 200
     *  1 + 200*1 = 201     200 + 200*1 = 400
     *  1 + 200*2 = 401     200 + 200*2 = 600
     *      ...                 ...
     */
    for (int i = 0; i < NUM_THREADS; i++) {
        int* row;
        row = (int*) malloc(sizeof(int)*2);

        *row = 1 + interval*i;
        *(row+1) = interval + interval*i;

        *(ranges+i) = row;
    }

    return ranges;
}

void print_arr(int** ranges) {
    printf("[ ");

    int* row;
    for (int i = 0; i < NUM_THREADS - 1; i++) {
        row = *(ranges + i);
        printf(" [%d, %d]", *row, *(row + 1));
    }

    row = *(ranges + NUM_THREADS - 1);
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
    ranges = create_ranges(100);
    print_arr(ranges);

    pthread_t threads[NUM_THREADS];
    int err_code;
    int* row;
    int* params;

    mutex = malloc_mutex(NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        row = *(ranges + i);
        params = generate_params(*row, *(row+1), i);

        err_code = pthread_create(&threads[i], NULL, print_numbers_in_range, (void*) params);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    free_mutex(mutex);

    printf("Thread main finished\n");
    pthread_exit(NULL);

    return 0;
}
