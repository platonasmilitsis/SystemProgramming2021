#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Record.h"
#include "SkipList.h"
#include "BloomFilter.h"
#include "VirusTable.h"
#include "HashTable.h"



//Function to create a new node
VirusListNode *create_vnode(Record  *record,int bloom_size, int bloom_k){
    VirusListNode *node = malloc(sizeof(VirusListNode));
    node->bloom = BloomFilter_Create(bloom_size,bloom_k);
    node->vaccinated=SkipList_Create();
    node->not_vaccinated=SkipList_Create();
    node->virus=strdup(record->virusName);
    if(strcmp(record->info,"YES")==0){
       node->vaccinated = SkipList_Insert(node->vaccinated,record);
       node->bloom = BloomFilter_Insert(node->bloom,record->CitizenId);
    }
    else{ 
        node->not_vaccinated = SkipList_Insert (node->not_vaccinated,record);
    }
    node->next=NULL;
    return node;
}



//Function to create a new list
VirusList *create_vlist(){
    VirusList *l = malloc(sizeof(VirusList));
    l->head=NULL;
    l->tail=NULL;
    l->num_elements=0;
    return l;
}

//Function to insert a new node to the list
VirusList *insert_vlist(VirusList *l,Record  *record,int bloom_size, int bloom_k){
    VirusListNode *cur = find_list_vnode(l,record->virusName);
    if(cur==NULL){
        cur = create_vnode(record,bloom_size,bloom_k);
        if (l->head == NULL){
            l->head=cur;
            l->tail=cur;
        }
        else{
            l->tail->next=cur;
            l->tail=cur;
        }
    }
    else{
        if(strcmp(record->info,"YES")==0){
            cur->vaccinated=SkipList_Insert(cur->vaccinated,record);
            cur->bloom= BloomFilter_Insert(cur->bloom,record->CitizenId);
        }
        else{
            cur->not_vaccinated=SkipList_Insert(cur->not_vaccinated,record);
        }
        
    }
    return l;
}

//Function to find an element inside a list
Record  *find_vlist(VirusList *l,char* id,char* virus_name){
    VirusListNode *cur = l->head;
    while(cur!=NULL){
        if(strcmp(virus_name,cur->virus)==0){
            Record *r=SkipList_Search(cur->vaccinated,id);
            if (r!=NULL) return r;
            r=SkipList_Search(cur->not_vaccinated,id);
            if (r!=NULL) return r;
        }
        
        cur = cur->next;
    }
    return NULL;
}

VirusListNode *find_list_vnode(VirusList *l,char* virus_name){
    VirusListNode *cur = l->head;
    while(cur!=NULL){
        if(strcmp(virus_name,cur->virus)==0){
           return cur;
        }
        cur = cur->next;
    }
    return NULL;
}


//Function to delete a given list
void delete_vlist(VirusList *l){
    if(l==NULL)
        return;

    if(l->head!=NULL && l->head==l->tail ){
        deleteNode(l->head);
        free(l);
        return;
    }

    VirusListNode *cur = l->head;
    while(cur->next!=NULL){
        VirusListNode *temp = cur;
        cur = cur->next;
        deleteNode(temp);
    }

    if(l->tail!=NULL){
        deleteNode(l->tail);
    }
    
    free(l);
}

void deleteNode(VirusListNode *node){
    
    BloomFilter_Delete(node->bloom);
    SkipList_Destroy(node->vaccinated);
    SkipList_Destroy(node->not_vaccinated);
    free(node->virus);
    free (node);
}

//Hash function lose lose
 unsigned long virus_hash(char *str, int num_buckets)
    {
    unsigned int hash = 0;
    int c;
    while (c = *str++)
        hash += c;

    return (hash%num_buckets);
    }

//Function to create a new hash table
VirusTable *create_vtable(int num_buckets,int bloom_size, int bloom_k){
    VirusTable *vt = malloc(sizeof(VirusTable));
    vt->num_buckets=num_buckets;
    vt->num_elements=0;
    vt->table=malloc(sizeof(VirusList*)*num_buckets);
    vt->bloom_size=bloom_size;
    vt->bloom_K=bloom_k;
    //Initialize every bucket as NULL
    for(int i=0;i<num_buckets;i++) vt->table[i]=NULL;

    return vt;
}

//Function to insert a new entry into the hash table
VirusTable *insert_vtable(VirusTable *vt,Record *record){
    int h = virus_hash(record->virusName,vt->num_buckets);
    //Search for the entry inside the correct list
    //if it exists increase the counter else create a new entry for the id

    //Given bucket is empty create a new list and insert the new entry
    if(vt->table[h]==NULL){
        vt->table[h] = create_vlist();
        vt->table[h] = insert_vlist(vt->table[h],record,vt->bloom_size,vt->bloom_K);
        return vt;
    }
    //Search for the id - if it does not exist create a new entry
    else{
        vt->table[h] = insert_vlist(vt->table[h],record, vt->bloom_size, vt->bloom_K);
        return vt;
        
    }
}

Record  *search_vtable(VirusTable *vt,char *id,char * virus_name){
   
    int h = virus_hash(virus_name,vt->num_buckets);
    if(vt->table[h]!=NULL){
        return find_vlist(vt->table[h],id,virus_name);        
    }
    
    return NULL;
}

VirusListNode *search_vtable_node(VirusTable *vt,char * virus_name){
   
    int h = virus_hash(virus_name,vt->num_buckets);
    if(vt->table[h]!=NULL){
        return find_list_vnode(vt->table[h],virus_name);        
    }
    return NULL;
}

char *package_bloomfilters_vlist(VirusList *vlist){
    VirusListNode *node = vlist->head;
    char *msg=NULL;
    while(node!=NULL){
        char *bloom = BloomFilter_Package(node->bloom,node->virus);
        if(msg==NULL){
            msg = strdup(bloom);
        }
        else{
            msg = realloc(msg,strlen(msg)+strlen(bloom)+1);
            strcat(msg,bloom);            
        }
        free(bloom);
        node = node->next;
    }

    return msg;
}

//Function to get all bloom filters in the table in string format
char *package_bloomfilters_vtable(VirusTable *vt){
    char *msg=NULL;
    for(int i=0;i<vt->num_buckets;i++){
        if(vt->table[i]!=NULL){
            char *blooms = package_bloomfilters_vlist(vt->table[i]);
            if(msg==NULL){
                msg = strdup(blooms);
            }
            else{
                msg = realloc(msg,strlen(msg)+strlen(blooms)+1);
                strcat(msg,blooms);
            }
            free(blooms);
        }
    }
    return msg;
}


//Function to delete a given table
void delete_vtable(VirusTable *vt){
    for(int i=0;i<vt->num_buckets;i++){
        if(vt->table[i]!=NULL){
            delete_vlist(vt->table[i]);
        }
    }
    free(vt->table);
    free(vt);
}

