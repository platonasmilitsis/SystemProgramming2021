#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Record.h"
#include "SkipList.h"
#include "BloomFilter.h"
#include "HashTable.h"





//Function to create a new node
ListNode *create_node(Record  *record){
    ListNode *node = malloc(sizeof(ListNode));
    node->record  = record;
    node->next=NULL;
    return node;
}

Record  *createRec(char *buffer){

    buffer=strtok(buffer,"\n\r");
    //allocate a record
    Record * r = malloc(sizeof(Record));
    char* segment;

    //assign the correct values

    //get the id
    segment = strtok(buffer, " ");
    r->CitizenId = strdup(segment);

    //get the name
    segment = strtok(NULL, " ");
    r->firstname = strdup(segment);

    //get the last name
    segment = strtok(NULL, " ");
    r->lastname = strdup(segment);
    
    //get the country
        segment = strtok(NULL, " ");
        r->Country = strdup(segment);

    //get the age
        segment = strtok(NULL, " ");
        r->age = atoi(segment);    

    //get the disease
        segment = strtok(NULL, " ");
        r->virusName = strdup(segment);

    //get the info yes or no
        segment = strtok(NULL, " ");
        r->info = strdup(segment);
    
    if(strcmp(r->info,"NO")==0){
        r->dateVaccinated=NULL;
        return r;
    }
    
    r->dateVaccinated=malloc(sizeof(Date));
    //get the entry date
     segment = strtok(NULL, " ");
   // printf("%s\n",segment);
    r->dateVaccinated->day = strtol(segment, NULL, 10);
    r->dateVaccinated->month = strtol(segment + 3, NULL, 10);
    r->dateVaccinated->year = strtol(segment + 6, NULL, 10);

    
    return r;
}

void delRec(Record *record){
  free(record->CitizenId);
  free(record->Country);
  free(record->firstname);
  free(record->lastname);
  free(record->info);  
  free(record->virusName);
  if(record->dateVaccinated!=NULL){
    free(record->dateVaccinated);
  }
  free(record);
}



//Function to create a new list
List *create_list(){
    List *l = malloc(sizeof(List));
    l->head=NULL;
    l->tail=NULL;
    l->num_elements=0;
    return l;
}

//Function to insert a new node to the list
List *insert_list(List *l,Record  *record){

    ListNode *node = create_node(record);

    if(l->head==NULL){

        l->head=l->tail=node;
        
        
    }
    else{
        l->tail->next=node;
        l->tail=node;
    }

    l->num_elements++;
    return l;
}

//Function to find an element inside a list
Record  *find_list(List *l,Record * record){
    ListNode *cur = l->head;
    while(cur!=NULL){
        if(cur->record->CitizenId==record->CitizenId)
            return cur->record;
        cur = cur->next;
    }
    return NULL;
}

//Helper function to print a given list
void print_list(List *l){
    ListNode *cur = l->head;
    while(cur!=NULL){
        PrintRecord(cur->record);
        cur = cur->next;
    }
}

//Function to delete a given list
void delete_list(List *l){
    if(l==NULL)
        return;

    if(l->head!=NULL && l->head==l->tail ){
        delRec(l->head->record);
        free(l->head);
        free(l);
        return;
    }

    ListNode *cur = l->head;
    while(cur->next!=NULL){
        ListNode *temp = cur;
        cur = cur->next;
        delRec(temp->record);
        free(temp);
    }

    if(l->tail!=NULL){
        delRec(l->tail->record);
        free(l->tail);
    }
    
    free(l);
}

//Hash function lose lose
 unsigned long hash(Record *record, int num_buckets)
    {
    unsigned int hash = 0;
    int c;
    char *str= record->CitizenId;
    while (c = *str++)
        hash += c;

    return (hash%num_buckets);
    }

//Function to create a new hash table
HashTable *create_table(int num_buckets){
    HashTable *ht = malloc(sizeof(HashTable));
    ht->num_buckets=num_buckets;
    ht->num_elements=0;
    ht->table=malloc(sizeof(List*)*num_buckets);
    //Initialize every bucket as NULL
    for(int i=0;i<num_buckets;i++) ht->table[i]=NULL;

    return ht;
}



//Function to insert a new entry into the hash table
HashTable *insert_table(HashTable *ht,Record **record,int *flag){
    *flag=0;
    int h = hash(*record,ht->num_buckets);
    //Search for the entry inside the correct list
    //if it exists increase the counter else create a new entry for the id

    //Given bucket is empty create a new list and insert the new entry
    if(ht->table[h]==NULL){
        ht->table[h] = create_list();
        ht->table[h] = insert_list(ht->table[h],*record);
        return ht;
    }
    //Search for the id - if it does not exist create a new entry
    else{
        
        ListNode *cur = ht->table[h]->head;
        while(cur!=NULL){
            if(strcmp(cur->record->CitizenId,(*record)->CitizenId)==0){
                if(strcmp(cur->record->virusName,(*record)->virusName)==0){
                    //duplicate id with different information
                    if((strcmp((*record)->info,"NO")==0) || (Check_Unique((*record),cur->record)==-1) || (strcmp(cur->record->info,"YES")==0)) {
                        printf("ERROR IN RECORD %s\n",(*record)->CitizenId);
                        delRec(*record);
                        *record=NULL;
                        return ht;
                    }
                    //no to yes
                    else{
                        *flag=1;
                        return ht;
                    }
                }
                else{
                    //different id same information
                    if(Check_Unique(cur->record,(*record))==-1){
                        printf("ERROR IN RECORD  %s\n",(*record)->CitizenId);
                        delRec(*record);
                        *record=NULL;
                        return ht;
                    }
                }
            }
            cur = cur->next;
        }
       // The entry does not exist insert it into the bucket
       
        ht->table[h] = insert_list(ht->table[h],*record);
        return ht;
        
    }
}

Record  * search_table(HashTable *ht,Record *record ){
   
    int h = hash(record,ht->num_buckets);
    if(ht->table[h]!=NULL){
        ListNode *cur = ht->table[h]->head;
        while(cur!=NULL){
            if(cur->record->CitizenId==record->CitizenId){
                return cur->record;
            }
            cur = cur->next;
        }
        
    }
    return NULL;
}

void print_table(HashTable *ht){
    
    for(int i=0 ; i<ht->num_buckets; i++){
        if(ht->table[i]!=NULL){
            printf("PRINT BUCKET %d:\n", i);
            print_list(ht->table[i]);
        }
    }
    

}

//Function to delete a given table
void delete_table(HashTable *ht){
    for(int i=0;i<ht->num_buckets;i++){
        if(ht->table[i]!=NULL){
            delete_list(ht->table[i]);
        }
    }
    free(ht->table);
    free(ht);
}



