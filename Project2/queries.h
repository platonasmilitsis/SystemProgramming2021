#ifndef QUERIES_H
#define QUERIES_H
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "SkipList.h"
#include "BloomFilter.h"
#include "LinkedList.h"
#include "bstlist.h"

int monitor_commands(HashTable *ht,VirusTable *vt,BstList *bst_l,int rfd,int wfd,int buffer_size);

int monitor_travelRequest(LinkedList *l,HashTable *ht,VirusTable *vt,BstList *bst_l,int rfd,int wfd,int buffer_size);

int monitor_travelStats(LinkedList *l,BstList *bst_l,int rfd,int wfd,int buffer_size);

int monitor_searchVaccination(LinkedList *l,HashTable *ht,VirusTable *vt,BstList *bst_l,int rfd,int wfd,int buffer_size);




#endif //QUERIES_H