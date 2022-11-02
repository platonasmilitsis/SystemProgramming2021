#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bstlist.h"

BstList *BstList_create(){
    BstList *bst_list = malloc(sizeof(BstList));
    bst_list->size=0;
    bst_list->head=NULL;
    bst_list->tail=NULL;
    return bst_list;
}


BstListNode *Create_bstlist_node(char *country,char *virus,Date *date,int flag){
    BstListNode *newBstlistnode= malloc (sizeof(BstListNode));
    newBstlistnode->country=strdup(country);
    newBstlistnode->virus=strdup(virus);
    newBstlistnode->bst = BSTnewNode(date,flag);
    newBstlistnode->next=NULL;
    return newBstlistnode; 
}

BstList *BstList_Insert(BstList *bst_list,char *country,char *virus,Date *date,int flag){
    BstNode *bst = BstList_Search(bst_list,country,virus);
    if(bst!=NULL){
        bst = BSTinsert(bst,date,flag);
    }
    else{
        BstListNode *node=Create_bstlist_node(country,virus,date,flag);

        if(bst_list->head==NULL){
            bst_list->head=bst_list->tail=node;
            node->next=NULL;
        }
        else{
            bst_list->tail->next=node;
            bst_list->tail=node;
        }

        bst_list->size++;
    }
    return bst_list;
}

//Get the number of requests for a given country for a virus in a given date range
void BstList_Request_Sum(BstList *bst_list, char* country,char *virus,Date *d1,Date *d2,int *accepted,int *declined){
    if(country==NULL){
        BstListNode *node=bst_list->head;
        while(node!=NULL){
            if(strcmp(node->virus,virus)==0){
                BSTsum(node->bst,d1,d2,accepted,declined);
            }
            node = node->next;
        }
    }
    else{
        BstListNode *node=bst_list->head;
        while(node!=NULL){
            if(strcmp(node->country,country)==0 && strcmp(node->virus,virus)==0){
                BSTsum(node->bst,d1,d2,accepted,declined);
            }
            node = node->next;
        }
    }
}

//Get the total amount of requests made
void BstList_Request_Sum_Simple(BstList *bst_list,int *accepted,int *declined){
    BstListNode *node=bst_list->head;
    while(node!=NULL){
        BSTsum_simple(node->bst,accepted,declined);
        node = node->next;
    }
}

BstNode *BstList_Search(BstList *bst_list, char* country,char *virus){
    BstListNode *node=bst_list->head;
    while(node!=NULL){
        if(strcmp(node->country,country)==0 && strcmp(node->virus,virus)==0)
            return node->bst;
        node = node->next;
    }
    return NULL;
}

void BstList_print(BstList *bst_list){
    BstListNode *node= bst_list->head;
    while(node!=NULL){
        printf("%s\n",node->country);
        node = node->next;
    }
}

void BstList_Destroy(BstList *bst_list){
    BstListNode *node=bst_list->head;
    while (node!=NULL){
        BstListNode *temp = node;
        node = node->next;
        BSTdestroy(temp->bst);
        free(temp->country);
        free(temp->virus);
        free(temp);
    }
    free(bst_list);
}