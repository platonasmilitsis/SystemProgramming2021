#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "VirusTable.h"
#include "Threads.h"
#include <dirent.h>
#include "StringList.h"
#include "parser.h"
#include "helpMonitorClient.h"

cyclicBuffer *cb_init(int bSize, pthread_mutex_t *locking_queue, pthread_cond_t *queue_nonempty,pthread_cond_t *queue_nonfull){
    cyclicBuffer *cb = malloc(sizeof(cyclicBuffer));
    cb->head = 0;
    cb->tail = -1;
    cb->counter = 0;
    cb->bufferSize = bSize;
    cb->path = malloc(sizeof(char *)*bSize);
    for(int i=0;i<bSize;i++){
        cb->path[i] = NULL;
    }

    cb->locking_queue = locking_queue;
    cb->queue_nonfull = queue_nonfull;
    cb->queue_nonempty = queue_nonempty;
    cb->threads_must_exit = 0;

    return cb;

}

cyclicBuffer *cb_place(cyclicBuffer *cb, char *path){
    pthread_mutex_lock(cb->locking_queue);
    pthread_cond_signal(cb->queue_nonempty);

    while(cb->counter >= cb->bufferSize){
        //printf(">> Found Buffer Full \n");
        pthread_cond_wait(cb->queue_nonfull, cb->locking_queue);
    }
    cb->tail = (cb->tail + 1) % cb->bufferSize;
    cb->path [cb->tail] = path;
    cb->counter++;

    
    pthread_mutex_unlock(cb->locking_queue);
    return cb;
}

char* cb_obtain(cyclicBuffer **cb){
    char* path = NULL;
    pthread_mutex_lock((*cb)->locking_queue);
    pthread_cond_signal((*cb)->queue_nonfull);
    if((*cb)->threads_must_exit == 1){

        pthread_mutex_unlock((*cb)->locking_queue);
        return NULL;
    
    }
    while ((*cb)->counter <= 0 && (*cb)->threads_must_exit == 0) {
        
        if((*cb)->threads_must_exit == 1){
            pthread_mutex_unlock((*cb)->locking_queue);
            return NULL;
        }

        //printf(">> Found Buffer Empty \n");
        pthread_cond_wait((*cb)->queue_nonempty, (*cb)->locking_queue);
    }
    path = (*cb)->path[(*cb)->head];
    (*cb)->head = ((*cb)->head + 1) % ((*cb)->bufferSize);
    (*cb)->counter--;
    pthread_mutex_unlock((*cb)->locking_queue);
    return path;

}


threadArgs *argsInit(cyclicBuffer *cb, VirusTable *vt, HashTable *ht,StringList *files,pthread_mutex_t *locking_queue,pthread_cond_t *queue_nonempty, pthread_cond_t *queue_nonfull,pthread_mutex_t *locking_structs,pthread_cond_t *structs_filled, int *counter){/*, pthread_cond_t *jobs_finished_cond*/
    threadArgs *args = malloc(sizeof(threadArgs));
    args->files = files;
    args->cb = cb;
    args->vt = vt;
    args->ht = ht;
    args->locking_queue = locking_queue;
    args->locking_structs = locking_structs;
    args->queue_nonempty = queue_nonempty;
    args->queue_nonfull = queue_nonfull;
    args->structs_filled=structs_filled;
    args->countries_cnt=counter;
    return args;
}

void *thread_fun(void *args){
    
    char *path;
    threadArgs *threadargs = (threadArgs * )args;
    
    pthread_mutex_t *locker = threadargs->locking_queue;
    pthread_mutex_t *locking_structs = threadargs->locking_queue;

    pthread_cond_t *queue_nonempty = threadargs->queue_nonempty;
    pthread_cond_t *queue_nonfull = threadargs->queue_nonfull;
    pthread_cond_t *structs_filled = threadargs->structs_filled;

    cyclicBuffer *cb = threadargs->cb;
    VirusTable *vt = threadargs->vt;
    HashTable *ht = threadargs->ht;
    StringList *files = threadargs->files;

    int *countries_cnt = threadargs->countries_cnt;

    while(1){
        path = cb_obtain(&cb);
        if(cb->threads_must_exit == 1)
            return NULL;
        else if(path == NULL){
            perror_exit("threads");
        }
        else{
            pthread_mutex_lock(locking_structs);
           
            struct dirent *de;
            DIR *dr = opendir(path);
            while ((de = readdir(dr)) != NULL){
                if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
                    files = StringList_Insert(files,de->d_name);
                    char *filepath=malloc(strlen(path)+strlen(de->d_name)+2);
                    strcpy(filepath,path);
                    strcat(filepath,"/");
                    strcat(filepath,de->d_name);
                    //Use parser to get records from the given file
                    parser(filepath,ht,vt);
                    free(filepath);
                }
            }
            closedir(dr);
            free(path);

            (*countries_cnt)--;
            if((*countries_cnt)==0)
                pthread_cond_signal(structs_filled);
            pthread_mutex_unlock(locking_structs);
        }
    
    }

}