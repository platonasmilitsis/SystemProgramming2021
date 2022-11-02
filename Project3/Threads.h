#ifndef THREADS_H
#define THREADS_H


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"

#include "VirusTable.h"
#include "StringList.h"

typedef struct cyclicBuffer{
    char **path;
    int head;
    int tail;
    int counter;
    int bufferSize;

    int threads_must_exit;

    pthread_mutex_t *locking_queue;
    pthread_cond_t *queue_nonempty;
    pthread_cond_t *queue_nonfull;
    
}cyclicBuffer;

typedef struct threadArgs{

    pthread_mutex_t *locking_queue;
    pthread_mutex_t *locking_structs;
    pthread_cond_t *queue_nonempty;
    pthread_cond_t *queue_nonfull;
    pthread_cond_t *structs_filled;

    int *countries_cnt;
    
    cyclicBuffer *cb;
    VirusTable *vt;
    HashTable *ht;
    StringList *files;
}threadArgs;


cyclicBuffer *cb_init(int bSize, pthread_mutex_t *locking_queue, pthread_cond_t *queue_nonempty,pthread_cond_t *queue_nonfull);
cyclicBuffer *cb_place(cyclicBuffer *cb, char *path);
char* cb_obtain(cyclicBuffer **cb);


threadArgs *argsInit(cyclicBuffer *cb, VirusTable *vt, HashTable *ht,StringList *files,pthread_mutex_t *locking_queue,pthread_cond_t *queue_nonempty, pthread_cond_t *queue_nonfull,pthread_mutex_t *locking_structs,pthread_cond_t *structs_filled, int *counter);

void* thread_fun(void *args);

#endif //THREADS_H