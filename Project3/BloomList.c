#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BloomList.h"

BloomList *BloomList_create(){
    BloomList *BList = malloc(sizeof(BloomList));
    BList->size=0;
    BList->head=NULL;
    BList->tail=NULL;
    return BList;
}

BloomListNode *Create_bloomlist_node(BloomFilter *bf,char *disease){
    BloomListNode *newBlistnode= malloc (sizeof(BloomListNode));
    newBlistnode->disease=strdup(disease);
    newBlistnode->bf = bf;
    newBlistnode->next=NULL;
    return newBlistnode; 
}

BloomList *BloomList_Insert(BloomList *BList,BloomFilter *bf,char *disease){
    BloomListNode *node=Create_bloomlist_node(bf,disease);

    if(BList->head==NULL){
        BList->head=BList->tail=node;
        node->next=NULL;
    }
    else{
        BList->tail->next=node;
        BList->tail=node;
    }

    BList->size++;
    return BList;
}

BloomFilter *BloomList_Search(BloomList *BList, char* disease){
    BloomListNode *node=BList->head;
    while(node!=NULL){
        if(strcmp(node->disease,disease)==0)
            return node->bf;
        node = node->next;
    }
    return NULL;
}

void BloomList_print(BloomList *BList){
    BloomListNode *node=BList->head;
    while(node!=NULL){
        printf("%s\n",node->disease);
        node = node->next;
    }
}

//Function to unpackage a string containing the bloom filters and return a list
//containing them
BloomList *BloomList_Unpack(char *msg,int Bloomsize,int K){
    //Create an empty bloom list 
    BloomList *blist = BloomList_create();
    char *package = strdup(msg);
    char *disease = strtok(package,"\n");

    while(disease!=NULL){
        BloomFilter *bf = BloomFilter_Create(Bloomsize,K);
        while(1){
            char *token = strtok(NULL,"\n");
            if(token==NULL || strcmp(token,"Fin")==0)
                break;
            unsigned char value = atoi(token);
            token = strtok(NULL,"\n");
            int pos = atoi(token);
            if(pos<Bloomsize)
                bf->Filter[pos]=value;
        }
        blist = BloomList_Insert(blist,bf,disease);
        disease = strtok(NULL,"\n");
    }

    free(package);
    return blist;
}

void BloomList_Destroy(BloomList *BList){
    BloomListNode *node=BList->head;
    while (node!=NULL){
        BloomListNode *temp = node;
        node = node->next;
        BloomFilter_Delete(temp->bf);
        free(temp->disease);
        free(temp);
    }
    free(BList);
}