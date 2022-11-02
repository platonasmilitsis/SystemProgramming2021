#include <stdio.h>
#include "SkipList.h"
#include "Record.h"
#include "stdlib.h"
#include "string.h"
#include "BloomFilter.h"
#include "VirusTable.h"
#include "HashTable.h"
#include "CountryList.h"


CountryList *CountryList_create(){
    CountryList *CList = malloc(sizeof(CountryList));
    CList->size=0;
    CList->head=NULL;
    CList->tail=NULL;
    return CList;
}

CListnode *Create_country_node(char* country,int age){
    CListnode *newclistnode= malloc (sizeof(CListnode));
    newclistnode->country=strdup(country);
    newclistnode->age020=0;
    newclistnode->age2040=0;
    newclistnode->age4060=0;
    newclistnode->age60p=0;
    newclistnode->total=1;
    if(age==1)
        newclistnode->age020++;
    else if(age==2)
        newclistnode->age2040++;
    else if(age==3)
        newclistnode->age4060++;
    else if(age==4)
        newclistnode->age60p++;
    newclistnode->next=NULL;
    return newclistnode; 
}

CountryList *CountryList_Insert(CountryList *CList, char* country,int age){
    CListnode *Cnode=Create_country_node(country,age);

    if(CList->head==NULL){
        CList->head=CList->tail=Cnode;
        Cnode->next=NULL;
    }
    else{
        CList->tail->next=Cnode;
        CList->tail=Cnode;
    }

    CList->size++;
    return CList;
}

//Search for the given country
CountryList *CountryList_update(CountryList *CList, char* country,int age){
    CListnode *Cnode=CList->head;
    while(Cnode!=NULL){
        if(strcmp(Cnode->country,country)==0){
            if(age==0){
                Cnode->total++;
            }
            else if(age==1){
                Cnode->age020++;
                Cnode->total++;
            }
            else if(age==2){
                Cnode->age2040++;
                Cnode->total++;
            }
            else if(age==3){
                Cnode->age4060++;
                Cnode->total++;
            }
            else if(age==4){
                Cnode->age60p++;
                Cnode->total++;
            }
            return CList;
        }
        Cnode = Cnode->next;
    }
    //Country does not exist
    CList = CountryList_Insert(CList,country,age);
    return CList;
}

void CountryList_print(CountryList *CList,int flag){
    CListnode *Cnode=CList->head;
    while(Cnode!=NULL){
        if(flag==0){
            printf("%s\n",Cnode->country);
            print_statistics(Cnode->age020,Cnode->age2040,Cnode->age4060,Cnode->age60p,Cnode->total);
            printf("\n");
        }
        else{
            int pop = Cnode->age020+Cnode->age2040+Cnode->age4060+Cnode->age60p;
            printf("%s %d %.2f %%\n",Cnode->country,pop,((float) ((float)pop*100.0)/(float)Cnode->total));
        }
        Cnode = Cnode->next;
    }
}


void CountryList_destroy(CountryList *CList){
    CListnode *node=CList->head;
    while (node!=NULL){
        CListnode *temp = node;
        node = node->next;
        free(temp->country);
        free(temp);
    }
    free(CList);
}
