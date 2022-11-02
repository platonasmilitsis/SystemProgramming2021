#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include "pipe_functions.h"

//Function to write a message into the pipe using a selected protocol
void write_to_pipe(int fd,char *msg,int bufferSize){

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
            write(fd,split_message,strlen(split_message));
        }

    }
    //Message can be sent correctly through the pipe without splitting it
    else{
        char msg_to_send[bufferSize];
        memset(msg_to_send,0,bufferSize);
        sprintf(msg_to_send,"(%d)%s$",(int)strlen(msg),msg);
        write(fd,msg_to_send,strlen(msg_to_send));
    }
}

//Function to read a message from the pipe according to the protocol
char *read_from_pipe(int fd,int bufferSize){
    //Read the message iteratively until it ends
    char *msg=NULL;
    while(1){
        char consumed[bufferSize];
        memset(consumed,0,bufferSize);
        char msg_part[bufferSize];
        memset(msg_part,0,bufferSize);
        read(fd,consumed,1);
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
                read(fd,num,1);
                if(strcmp(num,")")==0)
                    break;
                strcat(str_len,num);
            }
            int len = atoi(str_len);
            read(fd,msg_part,len);
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