#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex.h"

#define NUM_READERS 5
#define NUM_WRITERS 4
#define DATA_SIZE 30

char* dado;
int pos;
Mutex* mutexEscritores;
int posLeitoresMutex=NUM_WRITERS;


void appendChar(char** string, int* pos, char c) {
    (*string)[*pos]=c;
    (*string)[(*pos)+1] = '\0';
    (*pos)++;
}

void* writer(void* param) {
    int* thread_number;
    thread_number = param;

    

    if (*thread_number >= 0 && *thread_number < NUM_WRITERS) { // If it has one digit
        char str[2];
        sprintf(str, "%d", *thread_number);
        lock(mutexEscritores, *thread_number);

        printf("Thread Escritora %d escrevendo\n", *thread_number);

        appendChar(&dado, &pos, str[0]);
        printf("toString: %s\n", dado);

        unlock(mutexEscritores, *thread_number);
    }

    free(param);

    pthread_exit(NULL);

    return NULL;
}

void* reader(void* param) {
    int* thread_number;
    thread_number = param;

    if (*thread_number>=0 && *thread_number<NUM_READERS) {
        lock(mutexEscritores, posLeitoresMutex);

        printf("Thread Leitora %d lendo - %s\n", *thread_number, dado);

        unlock(mutexEscritores, posLeitoresMutex);
        
    }
}


int main() {
    dado = (char*)malloc(sizeof(char)*DATA_SIZE);
    pos = 0;

    pthread_t leitores[NUM_READERS];
    pthread_t escritores[NUM_WRITERS];
    mutexEscritores = malloc_mutex(NUM_WRITERS);

    int err_code;

    for(int i = 0; i < NUM_WRITERS; i++) {
        printf("escritor %d\n", i);
        int* thread_number = (int*) malloc(sizeof(int));
        *thread_number = i;
        err_code = pthread_create(&escritores[i], NULL, writer, (void*) thread_number);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }

    for(int i = 0; i < NUM_READERS; i++) {
        printf("leitor %d\n", i);
        int* thread_number = (int*) malloc(sizeof(int));
        *thread_number = i;

        err_code = pthread_create(&leitores[i], NULL, reader, (void*) thread_number);

        if (err_code){
            printf("ERROR code is %d\n", err_code);
            return -1;
        }
    }



    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(escritores[i], NULL);
    }

    printf("Thread main finished\n");
    pthread_exit(NULL);

    return 0;
}
