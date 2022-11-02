#ifndef COUNTRYLIST_H
#define COUNTRYLIST_H

#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "SkipList.h"
#include "BloomFilter.h"

typedef struct Countrynode {
    char *country;
    int total;
    int age020;
    int age2040;
    int age4060;
    int age60p;
    struct Countrynode * next;
}CListnode;

typedef struct countryList{
    int size;
    CListnode *head;
    CListnode *tail;
}CountryList;

CountryList *CountryList_create();
CountryList *CountryList_Insert(CountryList *CList, char* country,int age);
CListnode *Create_country_node(char* country, int age);
CountryList *CountryList_update(CountryList *CList, char* country,int age);
void CountryList_print(CountryList *CList,int flag);
void CountryList_destroy(CountryList *CList);

#endif //COUNTRYLIST_H