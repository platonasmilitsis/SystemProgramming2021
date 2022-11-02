#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "SkipList.h"
#include "CountryList.h"

//SkipList creation
SkipList *SkipList_Create(){
    
	SkipList *Sl = malloc(sizeof(SkipList));
    Sl->size=0;
    //Allocate space for sentinel node
    Sl->sentinel = malloc(sizeof(node));
    //Allocate space for sentinel node in each level
    Sl->sentinel->next = malloc(sizeof(node*)*N);
    //Initialize levels to NULL
    for(int i=0;i<N;i++) Sl->sentinel->next[i]=NULL;
    Sl->sentinel->data=malloc(sizeof(Record));
    Sl->sentinel->data->CitizenId = strdup("0");
    Sl->sentinel->currlevel=N;
    return Sl;
}

//Random return head or tail
int FlipCoin(){
    int Flip=rand()%2;
    return Flip;
}

//SkipList node creation
node *node_create(Record* value){
    node *Newnode= malloc(sizeof(node));
    Newnode->currlevel=1;
    //New level for head
    while (FlipCoin()!= TAIL){
        Newnode->currlevel++;
    }

    Newnode->next=malloc(sizeof(node*)*Newnode->currlevel);
    //Initializing new node
    for (int i=0 ; i<Newnode->currlevel ; i++){
       Newnode->next[i]=NULL;
    }
    Newnode->data=value;
    return Newnode;
}


//SkipList Insertion
SkipList *SkipList_Insert(SkipList *Sl,Record *value){
    //Calling function node_create to create a new node to insert
    node *Innode=node_create(value);
    //Allocating space for the table that records the addresses for the insertion
    node **table=malloc(sizeof(node*)*N);
    //Initializing table
    for (int i = 0; i < N; i++) {
        table[i]=NULL;
    }
    node *temp=Sl->sentinel;
    //For loop to change level
    for (int i = (Sl->sentinel->currlevel-1); i >= 0; i--) {
        //While loop to iterate in the current level
        while(temp->next[i]!=NULL && atoi(temp->next[i]->data->CitizenId) < atoi(Innode->data->CitizenId)){
            temp = temp->next[i];
        }
        //Keep the address of temp
        table[i]=temp;
    }

    //Does not exist
    if(temp->next[0]==NULL || strcmp(Innode->data->CitizenId,temp->next[0]->data->CitizenId)!=0){
        for (int i = 0; i < Innode->currlevel; i++){
            Innode->next[i]=table[i]->next[i];
            table[i]->next[i]=Innode;
        }
        Sl->size++;
    }
    else{
        free(table);
        free(Innode->next);
        free(Innode);
       
        return Sl;
    }
   
    free(table);
    return Sl;
}




//SkipList Search
Record  *SkipList_Search(SkipList *Sl,char* value){
    node *Snode=Sl->sentinel;
    for (int i = (Snode->currlevel-1); i >= 0; i--) {
        while (Snode->next[i]!=NULL && atoi (Snode->next[i]->data->CitizenId) < atoi(value)){
            Snode=Snode->next[i];
        }
       
    }
     if(Snode->next[0]!=NULL && strcmp(Snode->next[0]->data->CitizenId,value)==0){
            return Snode->next[0]->data;
        }
        else{
            return NULL;
        }
    
}


   
    

void SkipList_print(SkipList *Sl){
    node *cur = Sl->sentinel->next[0];
    while(cur!=NULL){
        for(int i=0;i<cur->currlevel;i++)
            printf("%s ",cur->data->CitizenId);
        printf("\n");
        cur = cur->next[0];
    }
}




SkipList *SkipList_Delete(SkipList *Sl,char *value){
    node *dtemp=Sl->sentinel;
    node **deltable=malloc(sizeof(node*)*N);
     for (int i = 0; i < N; i++) {
        deltable[i]=NULL;
    }
    for(int i=(dtemp->currlevel-1); i>=0 ; i--){
        while(dtemp->next[i]!=NULL && atoi(dtemp->next[i]->data->CitizenId)< atoi(value)){
            dtemp = dtemp->next[i];
        }
        deltable[i]=dtemp;
        
    }
    dtemp=dtemp->next[0];
    if(dtemp!=NULL && strcmp(dtemp->data->CitizenId,value)==0){
        for (int i = 0; i < dtemp->currlevel; i++)
        {
            deltable[i]->next[i] = dtemp->next[i]; 
        }
        
        Sl->size--;
    }
    free(dtemp->next);
    free(dtemp);
    free(deltable);
    return Sl;
}


void SkipList_Destroy(SkipList *Sl){
    node *Dnode=Sl->sentinel;
    free(Sl->sentinel->data->CitizenId);
    free(Sl->sentinel->data);
    node* next=Dnode->next[0];
    while (Dnode!=NULL){
        next=Dnode->next[0];
        free(Dnode->next);
        free(Dnode);
        Dnode=next;
    }
    
    free(Sl);
}


/* FOR QUERIES */

int Skiplist_search_in_range(SkipList *Sl,SkipList *non_vac,char *country,Date *date1,Date *date2,int flag){
    CountryList *l = CountryList_create();
    int age_range_0_20=0;
    int age_range_20_40=0;
    int age_range_40_60=0;
    int age_range_60_plus=0;
    int total=0;

    node *cur = Sl->sentinel->next[0];
    while(cur!=NULL){
        Record *r = cur->data;
        if(country==NULL){
            if(date1==NULL || (Compare_Dates(r->dateVaccinated,date1)>=0 && Compare_Dates(r->dateVaccinated,date2)<=0)){
                if(r->age>=0 && r->age<20)
                    CountryList_update(l,r->Country,1);
                else if(r->age<40)
                    CountryList_update(l,r->Country,2);
                else if(r->age<60)
                    CountryList_update(l,r->Country,3);
                else
                    CountryList_update(l,r->Country,4);

            }
        }
        else if(strcmp(r->Country,country)==0){
            if(date1==NULL || (Compare_Dates(r->dateVaccinated,date1)>=0 && Compare_Dates(r->dateVaccinated,date2)<=0)){
                if(r->age>=0 && r->age<20)
                    age_range_0_20++;
                else if(r->age<40)
                    age_range_20_40++;
                else if(r->age<60)
                    age_range_40_60++;
                else
                    age_range_60_plus++;
                total++;
            }
        }

        cur = cur->next[0];
    }

    //Search the non vaccinated skip list to get the total population status
    cur = non_vac->sentinel->next[0];
    while(cur!=NULL){
        Record *r = cur->data;
        if(country==NULL){
            if(date1==NULL || (Compare_Dates(r->dateVaccinated,date1)>=0 && Compare_Dates(r->dateVaccinated,date2)<=0))
                CountryList_update(l,r->Country,0);
        }
        else if(strcmp(r->Country,country)==0){
            total++;
        }
        cur = cur->next[0];
    }

    if(flag==0){
        if(country!=NULL){ 
            printf("%s\n",country);
            print_statistics(age_range_0_20,age_range_20_40,age_range_40_60,age_range_60_plus,total);
        }
        else{
            CountryList_print(l,flag);
        }
    }
    // /populationStatus
    else{
        if(country!=NULL){
            int pop = age_range_0_20+age_range_20_40+age_range_40_60+age_range_60_plus;
            printf("%s %d %.2f %%\n",country,pop,((float) ((float)pop*100.0)/(float)total));
        }
        else{
            CountryList_print(l,flag);
        }
    }
    CountryList_destroy(l);
    return 1;
}


int SkipList_Search_list(SkipList *Sl,char *virus_name){
    node *cur = Sl->sentinel->next[0];
    while(cur!=NULL){
        Record *r = cur->data;
        printf("%s %s %s %s %d\n",r->CitizenId,r->firstname,r->lastname,r->Country,r->age);
        cur = cur->next[0];
    }

    return 1;
}




