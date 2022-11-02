#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include "Record.h"

typedef struct Listnode {
    char *data;
    struct Listnode * next;
}Listnode;

typedef struct LinkedList{
    int size;
    Listnode *head;
    Listnode *tail;
}LinkedList;

LinkedList *List_create();
LinkedList *List_Insert(LinkedList *List,  char *value);
Listnode *create_list_node(char* value);
void List_destroy(LinkedList *List);

char * List_get_at(LinkedList *List,int index);

#endif //LINKEDLIST_H