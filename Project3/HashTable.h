#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_SIZE 1009


//Function to create a new entry
Record  *createRec(char* buffer);

//Struct for node
typedef struct listnode{
    Record  *record;
    struct listnode *next;
}ListNode;

//Function to create a new node
ListNode *create_node(Record  *record);

//Struct for list
typedef struct list{
    ListNode *head;
    ListNode *tail;
    int num_elements;
}List;

//Function to create a new list
List *create_list();

//Function to insert a new node to the list
List *insert_list(List *l,Record  *record);

//Function to find an element inside a list
Record  *find_list(List *l,Record *record);

//Helper function to print a given list
void print_list(List *l);

//Function to delete a given list
void delete_list(List *l);

void delRec(Record *record);


//Struct for the hashtable
typedef struct hashtable{
    int num_buckets;
    int num_elements;
    List **table;
}HashTable;

//Hash function
 unsigned long hash(Record *record, int num_buckets);
   

//Function to create a new hash table
HashTable *create_table(int num_buckets);

//Function to insert a new entry into the hash table
HashTable *insert_table(HashTable *ht,Record **record,int *flag);
Record  * search_table(HashTable *ht,Record *record );
void print_table(HashTable *ht);


//Function to delete a given table
void delete_table(HashTable *ht);

#endif //HASHTABLE_H
