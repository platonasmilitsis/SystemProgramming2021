#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h>  
#include <stdlib.h> 
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#include "helpMonitorClient.h"
#include "LinkedList.h"
#include "processList.h"
#include "bstlist.h"
#include "parser.h"
#include "pipe_functions.h"

//Function to initialize command line arguments
int inArguments(int argc,char **argv,int *numMonitors,int *socketBufferSize, int *cyclicBufferSize, int *sizeOfBloom,char **input_dir, int *numThreads){
	if (argc != 13){								
		printf("Invalid number of args, exiting\n");
		return -1;
	}

    for (int i = 1; i <= 11 ; i += 2 ){			
		if (!strcmp(argv[i], "-m" )){			
			*numMonitors = atoi(argv[i + 1]);
		}
		else if(!strcmp(argv[i], "-b")){
			*socketBufferSize = atoi(argv[i + 1]);
		}
        else if(!strcmp(argv[i], "-c")){
            *cyclicBufferSize = atoi(argv[i + 1]); 
        }
        else if(!strcmp(argv[i],"-s")){
            *sizeOfBloom = atoi(argv[i+1]);
        }
		else if(!strcmp(argv[i], "-i")){
			*input_dir = strdup(argv[i + 1]);
		}
        else if(!strcmp(argv[i],"-t")){
            *numThreads = atoi (argv[i+1]);
        }
		else{
			if(input_dir != NULL){
				free(*input_dir);
			}
			printf("Error in arguments (flags), exiting\n");
			return -1;
		}
	}

    return 1;
}

//Initialize array with ports
int *initPorts(int numMonitors,int seed){
    int *ports = malloc(sizeof(int)*numMonitors);
    for(int i=0;i<numMonitors;i++){
        ports[i]=seed+i;
    }
    return ports;
}

//Initialize server arguments
char **initServerArguments(int port,int num_threads,int sock_buf_size,int c_buf_size,int sizeOfBloom,char *countries){
    char **args = malloc(sizeof(char*)*13);
    
    args[0] = strdup("./monitorServer");
    args[1] = strdup("-p");
    char values[12]; memset(values,0,12);
    sprintf(values,"%d",port);
    args[2] = strdup(values); memset(values,0,12);

    args[3] = strdup("-t");
    sprintf(values,"%d",num_threads);
    args[4] = strdup(values); memset(values,0,12);

    args[5] = strdup("-b");
    sprintf(values,"%d",sock_buf_size);
    args[6] = strdup(values); memset(values,0,12);

    args[7] = strdup("-c");
    sprintf(values,"%d",c_buf_size);
    args[8] = strdup(values); memset(values,0,12);

    args[9] = strdup("-s");
    sprintf(values,"%d",sizeOfBloom);
    args[10]= strdup(values); memset(values,0,12);

    char *token = strtok(countries," ");
    int index=11;
    while(1){
        args[index]=strdup(token);
        token = strtok(NULL," ");
        if(token==NULL)
            break;
        index++;
        args=realloc(args,sizeof(char*)*(index+2));
    }
    args[index+1]=NULL;

    return args;
}


//Function to write a message into the pipe using a selected protocol
void write_to_socket(int fd,char *msg,int bufferSize){

    //Current message is larger than our buffer
    //the message must be split
    if(strlen(msg)>(bufferSize-9)){
    
        //Get the number of msgs
        int number_of_msgs = strlen(msg)/(bufferSize-9);
        int remaining_msg = 0;
        if(strlen(msg)%(bufferSize-9)!=0){
            number_of_msgs++;
            remaining_msg = strlen(msg)%(bufferSize-9);
        }

        for(int i=0;i<number_of_msgs;i++){
            int len=0;
            char split_message[bufferSize];
            memset(split_message,0,bufferSize);
            char msg_part[bufferSize-8];
            memset(msg_part,0,bufferSize-8);
            if(i==number_of_msgs-1 && remaining_msg!=0){
                memcpy(msg_part,msg+i*(bufferSize-9),remaining_msg);
                len=remaining_msg;
                sprintf(split_message,"(%d)%s$",len,msg_part);
            }
            else{
                memcpy(msg_part,msg+i*(bufferSize-9),bufferSize-9);
                len=bufferSize-9;
                if(i==number_of_msgs-1)
                    sprintf(split_message,"(%d)%s$",len,msg_part);
                else
                    sprintf(split_message,"(%d)%s",len,msg_part);
            }
            //Write the message after it was correctly formated
            int count = send(fd,split_message,strlen(split_message),0);
        }

    }
    //Message can be sent correctly through the pipe without splitting it
    else{
        char msg_to_send[bufferSize];
        memset(msg_to_send,0,bufferSize);
        sprintf(msg_to_send,"(%d)%s$",(int)strlen(msg),msg);
        int count = send(fd,msg_to_send,strlen(msg_to_send),0);
    }
}

//Function to read a message from the pipe according to the protocol
char *read_from_socket(int fd,int bufferSize){
    //Read the message iteratively until it ends
    char *msg=NULL;
    while(1){
        char consumed[bufferSize];
        memset(consumed,0,bufferSize);
        char msg_part[bufferSize];
        memset(msg_part,0,bufferSize);
        int count = recv(fd,consumed,1,0);
        if(strcmp(consumed,"$")==0)
            break;
        else if(strcmp(consumed,"(")==0){
            //Clear the buffer and get the messages length
            memset(consumed,0,bufferSize);
            char num[2];
            char str_len[6]; 
            memset(str_len,0,6);
            while(1){
                memset(num,0,2);
                int count = recv(fd,num,1,0);
                if(strcmp(num,")")==0)
                    break;
                strcat(str_len,num);
            }
            int len = atoi(str_len);
            int count = recv(fd,msg_part,len,0);
            if(msg==NULL){
                msg=strdup(msg_part);
            }
            else{
                msg = realloc(msg,strlen(msg)+strlen(msg_part)+2);
                strcat(msg,msg_part);
            }
        }
    }

    return msg;
}


//error and exit function
void perror_exit(char *message){
    perror(message);
    exit(EXIT_FAILURE);
}


