#ifndef VIRUSTABLE_H
#define VIRUSTABLE_H

#define MAX_SIZE 1009
#include "SkipList.h"
#include "BloomFilter.h"



//Struct for node

typedef struct virus_node{
    BloomFilter *bloom;
    SkipList *vaccinated;
    SkipList *not_vaccinated;
    char *virus;
    struct virus_node *next;
}VirusListNode;



//Function to create a new node
VirusListNode *create_vnode(Record  *record,int bloom_size, int bloom_k);

//Struct for list
typedef struct virus_list{
    VirusListNode *head;
    VirusListNode *tail;
    int num_elements;
}VirusList;

//Function to create a new list
VirusList *create_vlist();

//Function to insert a new node to the list
VirusList *insert_vlist(VirusList *l,Record  *record,int bloom_size, int bloom_k);

//Function to find an element inside a list
Record  *find_vlist(VirusList *l,char* id,char* virus_name);

VirusListNode *find_list_vnode(VirusList *l,char* virus_name);

//Function to delete a given list
void delete_vlist(VirusList *l);

void deleteNode(VirusListNode *node);

//Struct for the hashtable
typedef struct virustable{
    int num_buckets;
    int num_elements;
    VirusList **table;
    int bloom_size;
    int bloom_K;

}VirusTable;

//Hash function
 unsigned long virus_hash(char *str, int num_buckets);
   

//Function to create a new hash table
VirusTable *create_vtable(int num_buckets,int bloom_size, int bloom_k);

//Function to insert a new entry into the hash table
VirusTable *insert_vtable(VirusTable *vt,Record *record);
Record  * search_vtable(VirusTable *vt,char* id, char* virus_name );
void print_vtable(VirusTable *vt);
VirusListNode *search_vtable_node(VirusTable *vt,char * virus_name);

//Function to delete a given table
void delete_vtable(VirusTable *vt);

#endif //VIRUSTABLE_H
