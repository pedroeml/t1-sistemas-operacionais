#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex.h"

#define NUM_READERS 5
#define NUM_WRITERS 4

char* data;
int last_char_index;
Mutex* mutex;
int mutex_readers_index;

void append_char(char **string, int i, char c) {
    (*string)[i] = c;
    (*string)[i+1] = '\0';
}

void* writer(void* param) {
    int* thread_number;
    thread_number = param;

    if (*thread_number >= 0 && *thread_number < 10) { // If it has one digit, so append_char can be done
        char str[2];
        sprintf(str, "%d", *thread_number);
        lock(mutex, *thread_number);
        append_char(&data, last_char_index++, str[0]);
        printf("Writer %d writing: \"%s\"\n", *thread_number, data);
        unlock(mutex, *thread_number);
    }

    free(param);

    pthread_exit(NULL);

    return NULL;
}

void* reader(void* param) {
    int* thread_number;
    thread_number = param;

    lock(mutex, mutex_readers_index);
    printf("Reader %d reading: \"%s\"\n", *thread_number, data);
    unlock(mutex, mutex_readers_index);

    free(param);

    pthread_exit(NULL);

    return NULL;
}

int create_threads(bool is_writer, pthread_t* threads, int length) {
    int err_code;

    for (int i = 0; i < length; i++) {
        printf(is_writer ? "Writer %d has been created!\n" : "Reader %d has been created!\n", i);
        int* thread_number = (int*) malloc(sizeof(int));
        *thread_number = i;

        err_code = pthread_create((threads + i), NULL, is_writer ? writer : reader, (void*) thread_number);

        if (err_code) {
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    return 0;
}

void join_all(pthread_t* threads, int length) {
    for (int i = 0; i < length; i++) {
        pthread_join(*(threads+i), NULL);
    }
}

int readers_writers() {
    data = (char*) malloc(sizeof(char)*(NUM_WRITERS+1));
    last_char_index = 0;
    mutex_readers_index = NUM_WRITERS;

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    mutex = malloc_mutex(NUM_WRITERS);

    int err_code_writers, err_code_readers;

    err_code_writers = create_threads(true, &(writers[0]), NUM_WRITERS);
    err_code_readers = create_threads(false, &(readers[0]), NUM_READERS);

    if (err_code_writers != 0 || err_code_readers != 0)
        return -1;

    join_all(&(writers[0]), NUM_WRITERS);
    join_all(&(readers[0]), NUM_READERS);

    printf("Readers-Writers finished\n");

    return 0;
}

int main() {
    return readers_writers();
}
