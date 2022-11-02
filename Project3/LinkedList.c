#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Record.h"
#include "LinkedList.h"

//LIst creation
LinkedList *List_create(){
    LinkedList *List = malloc(sizeof(LinkedList));
    List->size=0;

    List->head=NULL;
    List->tail=NULL;
    return List;
}

//list node creation
Listnode *create_list_node(char* value){
    Listnode *newlistnode= malloc (sizeof(Listnode));
    newlistnode->data=strdup(value);
    newlistnode->next=NULL;
    return newlistnode; 
}


//List insertion
LinkedList *List_Insert(LinkedList *List, char* value){
    Listnode *Node=create_list_node(value);

    if(List->head==NULL){
       List->head=Node;
       List->tail=Node;
       Node->next=NULL;
    }
    else{
        List->tail->next=Node;
        List->tail=Node;
    }

    List->size++;
    return List;
}

//function to access an element of list
char *List_get_at(LinkedList *List,int index){
    if(index<0 || index>List->size){
        return NULL;
    }
    int i=0;
    Listnode *cur=List->head;
    while(cur!=NULL){
        if(i==index){
            return cur->data;
        }
        cur=cur->next;
        i++;
    }
    return NULL;
}

//destroy list
void List_destroy(LinkedList *List){
    Listnode *node=List->head;
    while (node!=NULL){
        Listnode *temp = node;
        node = node->next;
        free(temp->data);
        free(temp);
    }
    free(List);
}


