#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdio.h>
#include <math.h>
#include "Record.h"

#define N (int)log(7594000000.0)

typedef enum {HEAD,TAIL}Coin;

typedef struct node{
    Record* data;
    struct node **next;
    int currlevel;
}node;

typedef struct SkipList{
   node *sentinel;
   int size;
   int level; 
}SkipList;

SkipList *SkipList_Create();
int FlipCoin();
node *node_create(Record* value );
SkipList *SkipList_Insert(SkipList *Sl,Record * value);
void SkipList_print(SkipList *Sl);
Record *SkipList_Search(SkipList *Sl,char* value);
SkipList *SkipList_Delete(SkipList *Sl,char *value);
void SkipList_Destroy(SkipList *Sl);

//Queries
int Skiplist_search_in_range(SkipList *Sl,SkipList *non_vac,char* country,Date *date1,Date *date2,int flag);
int SkipList_Search_list(SkipList *Sl,char *virus_name);


#endif //SKIPLIST_H