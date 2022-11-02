#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringList.h"

StringList *StringList_create(){
    StringList *SList = malloc(sizeof(StringList));
    SList->size=0;
    SList->head=NULL;
    SList->tail=NULL;
    return SList;
}

StringNode *Create_string_node(char *value){
    StringNode *newSlistnode= malloc (sizeof(StringNode));
    newSlistnode->value=strdup(value);
    newSlistnode->next=NULL;
    return newSlistnode; 
}

StringList *StringList_Insert(StringList *SList, char* value){
    StringNode *node=Create_string_node(value);

    if(SList->head==NULL){
        SList->head=SList->tail=node;
        node->next=NULL;
    }
    else{
        SList->tail->next=node;
        SList->tail=node;
    }

    SList->size++;
    return SList;
}

int StringList_Search(StringList *SList, char* value){
    StringNode *node=SList->head;
    while(node!=NULL){
        if(strcmp(node->value,value)==0)
            return 1;
        node = node->next;
    }
    return 0;
}

void StringList_print(StringList *SList){
    StringNode *node=SList->head;
    while(node!=NULL){
        printf("%s\n",node->value);
        node = node->next;
    }
}

char *StringList_to_string(StringList *SList){
    char *msg = NULL;
    StringNode *node=SList->head;
    while(node!=NULL){
        if(msg==NULL) msg=strdup(node->value);
        else{
            msg = realloc(msg,strlen(msg)+strlen(node->value)+2);
            strcat(msg,"\n");
            strcat(msg,node->value);
        }
        node = node->next;
    } 

    return msg;   
}

void StringList_Destroy(StringList *SList){
    StringNode *node=SList->head;
    while (node!=NULL){
        StringNode *temp = node;
        node = node->next;
        free(temp->value);
        free(temp);
    }
    free(SList);
}

//Pop first item from StringList and get it's path
char *StringList_popPath(StringList *SList, char *input_dir){
    
    char *path;
    StringNode *temp;
    temp = SList->head;
    SList->head = SList->head->next;
    temp->next = NULL;


    path=malloc(strlen(temp->value)+strlen(input_dir)+2);


    strcpy(path,input_dir);
    strcat(path,"/");
    strcat(path,temp->value);

    free(temp);
    return path;

}
