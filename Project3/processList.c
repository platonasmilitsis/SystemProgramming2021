#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "processList.h"
#include <signal.h>

ProcessList *ProcessList_create(){
    ProcessList *PList = malloc(sizeof(ProcessList));
    PList->size=0;
    PList->head=NULL;
    PList->tail=NULL;
    return PList;
}

ProcessListNode *Create_processList_node(pid_t prid){
    ProcessListNode *newPlistnode= malloc (sizeof(ProcessListNode));
    newPlistnode->pid=prid;
    newPlistnode->blist=NULL;
    newPlistnode->slist=NULL;
    newPlistnode->next=NULL;
    return newPlistnode; 
}

ProcessList *ProcessList_Insert(ProcessList *PList, pid_t prid){
    ProcessListNode *node=Create_processList_node(prid);

    if(PList->head==NULL){
        PList->head=PList->tail=node;
        node->next=NULL;
    }
    else{
        PList->tail->next=node;
        PList->tail=node;
    }

    PList->size++;
    return PList;
}

//Function to add a string list to a node with the given pid
ProcessList *ProcessList_add_country(ProcessList *PList,pid_t prid,char *country){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        if(node->pid == prid){
            if(node->slist==NULL){
                node->slist = StringList_create();
            }
            node->slist = StringList_Insert(node->slist,country);
            return PList;
        }
        node = node->next;
    }
    return PList;
}

//Function to add a bloom list for a given pid
ProcessList *ProcessList_add_bloomlist(ProcessList *PList,pid_t prid,BloomList *blist){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        if(node->pid == prid){
            if(node->blist!=NULL) BloomList_Destroy(node->blist);
            node->blist = blist;
            return PList;
        }
        node = node->next;
    }
    return PList;
}

int ProcessList_Search(ProcessList *PList,char *country){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        if(StringList_Search(node->slist,country)==1)
            return node->pid;
        node = node->next;
    }
    return -1;
}


void ProcessList_print(ProcessList *PList){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        printf("%d :\n",node->pid);
        if(node->slist!=NULL)
            StringList_print(node->slist);
        node = node->next;
    }
}

//Function to send a signal to all processes
int ProcessList_Send_Signal_To_ALL(ProcessList *PList,int signo){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        kill(node->pid,signo);
        node = node->next;
    }
}

void ProcessList_Destroy(ProcessList *PList){
    ProcessListNode *node=PList->head;
    while (node!=NULL){
        ProcessListNode *temp = node;
        node = node->next;
        StringList_Destroy(temp->slist);
        BloomList_Destroy(temp->blist);
        free(temp);
    }
    free(PList);
}

char *ProcessList_to_string(ProcessList *PList){
    char *msg=NULL;
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        char *countries = StringList_to_string(node->slist);
        if(msg==NULL) msg = strdup(countries);
        else{
            msg = realloc(msg,strlen(msg)+strlen(countries)+2);
            strcat(msg,"\n");
            strcat(msg,countries);
        }
        free(countries);
        node = node->next;
    }

    return msg;
}

int ProcessList_search_blooms(ProcessList *PList,char *id,char *country,char *virus){
    ProcessListNode *node=PList->head;
    while(node!=NULL){
        if(StringList_Search(node->slist,country)==1){
            BloomFilter *bf = BloomList_Search(node->blist,virus);
            if(bf==NULL) return 0;
            else return BloomFilter_Search(bf,id);
        }
        node = node->next;
    }
    return 0;
}