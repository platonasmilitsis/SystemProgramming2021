#ifndef BLOOMLIST_H
#define BLOOMLIST_H

#include "BloomFilter.h"

typedef struct BloomListNode {
    char *disease;
    BloomFilter *bf;
    struct BloomListNode *next;
}BloomListNode;

typedef struct BloomList{
    int size;
    BloomListNode *head;
    BloomListNode *tail;
}BloomList;

BloomList *BloomList_create();
BloomList *BloomList_Insert(BloomList *BList,BloomFilter *bf,char *disease);
BloomListNode *Create_bloomlist_node(BloomFilter *bf,char *disease);
BloomFilter *BloomList_Search(BloomList *BList, char* disease);
BloomList *BloomList_Unpack(char *msg,int Bloomsize,int K);
void BloomList_Destroy(BloomList *BList);
void BloomList_print(BloomList *BList);


#endif //BLOOMLIST_H