#ifndef BUSYWAITMUTEX_MUTEX_H
#define BUSYWAITMUTEX_MUTEX_H

#include <stdbool.h>

typedef struct mutex {
    int number_threads;
    int* turns;
    bool* flags;
} Mutex;

Mutex* malloc_mutex(int number_threads);

void lock(Mutex* mutex, int thread_number);

void unlock(Mutex* mutex, int thread_number);

void free_mutex(Mutex* mutex);

#endif //BUSYWAITMUTEX_MUTEX_H
