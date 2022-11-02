#ifndef HELPMONITORCLIENT_H
#define HELPMONITORCLIENT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "processList.h"
#include "bstlist.h"



int inArguments(int argc,char **argv,int *numMonitors,int *socketBufferSize, int *cyclicBufferSize, int *sizeOfBloom,char **input_dir, int *numThreads);
void perror_exit(char *message);
char *read_from_socket(int fd,int bufferSize);
void write_to_socket(int fd,char *msg,int bufferSize);
int *initPorts(int numMonitors,int seed);
char **initServerArguments(int port,int num_threads,int sock_buf_size,int c_buf_size,int sizeOfBloom,char *countries);



#endif //HELPMONITORCLIENT_H