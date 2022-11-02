#ifndef BSTLIST_H
#define BSTLIST_H

#include "bst.h"
#include "Record.h"

typedef struct bstlistNode{
    char *country;
    char *virus;
    BstNode *bst;
    struct bstlistNode *next;
}BstListNode;


typedef struct BstList{
    int size;
    BstListNode *head;
    BstListNode *tail;
}BstList;

BstList *BstList_create();
BstListNode *Create_bstlist_node(char *country,char *virus,Date *date,int flag);
BstList *BstList_Insert(BstList *bst_list,char *country,char *virus,Date *date,int flag);
BstNode *BstList_Search(BstList *bst_list, char* country,char *virus);
void BstList_Request_Sum(BstList *bst_list, char* country,char *virus,Date *d1,Date *d2,int *accepted,int *declined);
void BstList_Request_Sum_Simple(BstList *bst_list,int *accepted,int *declined);
void BstList_print(BstList *bst_list);
void BstList_Destroy(BstList *bst_list);


#endif //BSTLIST_H