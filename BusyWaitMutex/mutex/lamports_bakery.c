#include <stdlib.h>
#include <stdio.h>
#include "lamports_bakery.h"

/***
 * Busy wait mutual exclusion for N > 2 threads: Lamport's bakery algorithm
 *
 * The lock() and unlock() functions were based on the following sources:
 *
 * https://github.com/LefterisXris/Lamport-s-Bakery-Algorithm-Java/blob/master/Bakery.java
 * https://github.com/mjyeaney/BakeryAlgorithm/blob/master/src/Bakery.cs
 * https://github.com/BenYoshi/LamportBakeryAlgorithm/blob/master/src/Customer.java
 *
 */

bool* flags_init(int length) {
    bool* flags;
    flags = (bool*) malloc(sizeof(bool)*length);

    for (int i = 0; i < length; i++)
        *(flags+i) = false;

    return flags;
}

int* turns_init(int length) {
    int* turns;
    turns = (int*) malloc(sizeof(int)*length);

    for (int i = 0; i < length; i++)
        *(turns+i) = 0;

    return turns;
}

Mutex* malloc_mutex(int number_threads) {
    Mutex* mutex;
    mutex = (Mutex*) malloc(sizeof(Mutex));

    if (mutex == NULL) {
        printf("Malloc error on malloc_mutex function!");
        return NULL;
    }

    bool* flags;
    flags = flags_init(number_threads);

    int* turns;
    turns = turns_init(number_threads);

    mutex->number_threads = number_threads;
    mutex->flags = flags;
    mutex->turns = turns;

    return mutex;
}

void free_mutex(Mutex* mutex) {
    free(mutex->flags);
    free(mutex->turns);
    free(mutex);
}

int max_value(int* values, int length) {
    int max = 0;

    for (int i = 0; i < length; i++) {
        int value = *(values+i);
        max = value > max ? value : max;
    }

    return max;
}

void set_flags(Mutex* mutex, int thread_number, bool value) {
    bool* flags;
    flags =  mutex->flags;

    *(flags+thread_number) = value;

    printf("Thread %d set flags[%d]=%s\n", thread_number, thread_number, value ? "true" : "false");
}

bool get_flags(Mutex* mutex, int i) {
    bool* flags;
    flags =  mutex->flags;

    return *(flags+i);
}

void set_turns(Mutex* mutex, int thread_number, int value) {
    int* turns;
    turns = mutex->turns;

    *(turns+thread_number) = value;

    printf("Thread %d set turns[%d]=%d\n", thread_number, thread_number, value);
}

int get_turns(Mutex* mutex, int i) {
    int* turns;
    turns = mutex->turns;

    return *(turns+i);
}

void lock(Mutex* mutex, int thread_number) {
    set_flags(mutex, thread_number, true);

    set_turns(mutex, thread_number, 1 + max_value(mutex->turns, mutex->number_threads));

    set_flags(mutex, thread_number, false);

    for (int i = 0; i < mutex->number_threads; i++) {
        if (i != thread_number) {
            while (get_flags(mutex, i));
            while (get_turns(mutex, i) != 0 && (get_turns(mutex, i) < get_turns(mutex, thread_number) || (get_turns(mutex, i) == get_turns(mutex, thread_number) && i < thread_number)));
        }
    }
}

void unlock(Mutex* mutex, int thread_number) {
    set_turns(mutex, thread_number, 0);
}
