#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "mutex.c"

#define NUM_READERS 5
#define NUM_WRITERS 2
#define DATA_SIZE 30

char* dado;
int pos;
Mutex* mutexEscritores;

void appendChar(char** string, int* pos) {
    (*string)[*pos]='a';
    (*string)[(*pos)+1] = '\0';
    (*pos)++;
}

void* write(void* param) {
    int* thread_number = param;
    lock(mutexEscritores, *thread_number);

    printf("Thread Escritora %d escrevendo\n", *thread_number);
    appendChar(&dado, &pos);
    printf("%s\n", dado);
    
    unlock(mutexEscritores, *thread_number);
}

int main() {
    dado = (char*)malloc(sizeof(char)*DATA_SIZE);
    pos = 0;

    pthread_t leitores[NUM_READERS];
    pthread_t escritores[NUM_WRITERS];
    mutexEscritores = malloc_mutex(NUM_WRITERS);

    int err_code;
    int i;
    int* argument=(int*)malloc(sizeof(int));
    for(i=0; i<NUM_WRITERS; i++) {
        *argument=i;
        printf("%d", *argument);
        err_code = pthread_create(&escritores[i], NULL, write, (void*)argument );

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
