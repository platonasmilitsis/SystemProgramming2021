#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "SkipList.h"
#include "BloomFilter.h"
#include "LinkedList.h"
#include "Record.h"
#include "queries.h"
#include "bstlist.h"
#include "pipe_functions.h"

int monitor_commands(HashTable *ht,VirusTable *vt,BstList *bst_l,int sock,int buffer_size){

    char *command = read_from_pipe(sock,buffer_size);
    LinkedList *l = tokenize_input(command);

    //Get the command type
    char *type = List_get_at(l,0);

    if(strcmp(type,"/travelRequest")==0){
        monitor_travelRequest(l,ht,vt,bst_l,sock,buffer_size);
        free(command);
        List_destroy(l);
        return 1;
    }
    else if(strcmp(type,"/searchVaccinationStatus")==0){
        monitor_searchVaccination(l,ht,vt,bst_l,sock,buffer_size);
        free(command);
        List_destroy(l);
        return 1;
    }
    else if(strcmp(type,"/exit")==0){
        free(command);
        List_destroy(l);
        return 0;
    }
    else{
        printf("\nERROR\n\n");
        free(command);
         List_destroy(l);
        return 1;

    }

    // free(command);
    // List_destroy(l);
}


/*------------- Query travelRequest ------------------*/

int monitor_travelRequest(LinkedList *l,HashTable *ht,VirusTable *vt,BstList *bst_l,int sock,int buffer_size){
    //Get the data from the list
    char *id = List_get_at(l,1);
    Date *date = malloc(sizeof(Date));
    parse_date(List_get_at(l,2),date);
    char *countryFrom = List_get_at(l,3);
    char *countryTo = List_get_at(l,4);
    char *virus = List_get_at(l,5);

    //Get the current record
    Record *r = search_vtable(vt,id,virus);
    char *msg=NULL;
    if(r!=NULL){
        int bst_flag=1;
        bst_l=BstList_Insert(bst_l,countryTo,virus,date,bst_flag);
        char string_date[12]; memset(string_date,0,12);
        sprintf(string_date,"%d-%d-%d",r->dateVaccinated->day,r->dateVaccinated->month,r->dateVaccinated->year);
        msg = malloc(strlen("YES")+strlen(string_date)+2);
        strcpy(msg,"YES ");
        strcat(msg,string_date);
    }
    //Request Denied - No Record
    else{
        int bst_flag=0;
        bst_l=BstList_Insert(bst_l,countryTo,virus,date,bst_flag);
        msg=strdup("NO");
    }

    //Write the message to travel Monitor
    write_to_pipe(sock,msg,buffer_size);
    free(msg);

    free(date);
    return 1;
}

int monitor_searchVaccination(LinkedList *l,HashTable *ht,VirusTable *vt,BstList *bst_l,int sock,int buffer_size){
    //Get the data from the list
    char *msg = NULL;
    char *id = List_get_at(l,1);
    char buffer[256];

    for(int i=0; i<vt->num_buckets;i++){
        if(vt->table[i]!=NULL){
            VirusListNode *cur=vt->table[i]->head;
            if(cur==NULL) {
                break;
            }
            Record *rec = SkipList_Search(cur->vaccinated,id);
            if(rec!=NULL && strcmp(rec->CitizenId,id)==0){
                memset(buffer,0,256);
                if(msg==NULL){ 
                    sprintf(buffer,"%s %s %s %s\nAGE %d\n",rec->CitizenId,rec->firstname,rec->lastname,rec->Country,rec->age);
                    msg = strdup(buffer);
                    memset(buffer,0,256);
                }
                sprintf(buffer,"%s VACCINATED ON %d-%d-%d\n",rec->virusName,rec->dateVaccinated->day,rec->dateVaccinated->month,rec->dateVaccinated->year);
                msg = realloc(msg,strlen(msg)+strlen(buffer)+1);
                strcat(msg,buffer);

            }
            else{
                rec= SkipList_Search (cur->not_vaccinated,id);
                if(rec!=NULL && strcmp(rec->CitizenId,id)==0 ){
                    memset(buffer,0,256);
                    if(msg==NULL){ 
                        sprintf(buffer,"%s %s %s %s\nAGE %d\n",rec->CitizenId,rec->firstname,rec->lastname,rec->Country,rec->age);
                        msg = strdup(buffer);
                        memset(buffer,0,256);
                    }
                    sprintf(buffer,"%s NOT YET VACCINATED\n",rec->virusName);
                    msg = realloc(msg,strlen(msg)+strlen(buffer)+1);
                    strcat(msg,buffer);
                }
            }

            cur=cur->next;
        }
    }

    //ID not found in this Monitor
    if(msg==NULL)
        msg = strdup("None");

    //Write the message to travel Monitor
    write_to_pipe(sock,msg,buffer_size);
    free(msg);

    return 1;
}
