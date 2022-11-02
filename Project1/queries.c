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

int menu(HashTable *ht,VirusTable *vt){
    char *line = NULL;
    size_t len = 0;


    while(1){
        getline(&line, &len, stdin);
        //Remove change line characters
        line = strtok(line,"\n\r");
        LinkedList *l = tokenize_input(line);
        
        //Get the command type
        char *type = List_get_at(l,0);

        if(strcmp(type,"/exit")==0){
            List_destroy(l);
            free(line);
            return 1;
        }
        else if(strcmp(type,"/popStatusByAge")==0){
            popStatusByAge(vt,l);
        }
        else if(strcmp(type,"/populationStatus")==0){
            populationStatus(vt,l);
        }
        else if(strcmp(type,"/vaccineStatusBloom")==0){
            vaccineStatusBloom(vt,l);
        }
        else if(strcmp(type,"/vaccineStatus")==0){
                vaccineStatus(vt,l);
        }
        else if(strcmp(type,"/insertCitizenRecord")==0){
                insertCitizenRecord(ht,vt,l);
        }
        else if(strcmp(type,"/list-nonVaccinated-Persons")==0){
               list_non_vaccinated(vt,l);
        }
        else if(strcmp(type,"/vaccinateNow")==0){
               vaccinateNow(vt,l);
        }
        else{
            printf("\nERROR\n\n");
        }

        List_destroy(l);
    }

    free(line);
    return 1;
}





/*---------- Query vaccineStatusBloom Implementation -------------*/

int vaccineStatusBloom(VirusTable *vt,LinkedList *l){
    int status=1;
    status=parse_bloom(l); 
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status=execute_bloom(vt,l);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    return status;
}

int parse_bloom(LinkedList *l){
    if(l->size!=3) return -1;
    if(l->size==3){
        if(is_id(List_get_at(l,1))==-1) return -1;
        if(is_virus(List_get_at(l,2))==-1) return -1;
    }
    return 1;
}

int execute_bloom(VirusTable *vt,LinkedList *l){
    char *id = List_get_at(l,1);
    char *virus_name = List_get_at(l,2);
    VirusListNode *node = search_vtable_node(vt,virus_name);
    if(node==NULL) return -1;
    printf("\n");
    if( BloomFilter_Search(node->bloom,id)==1){
        printf("MAYBE");
    }
    else{
        printf("NOT VACCINATED");
    }
    printf("\n\n");
}

/*---------- Query vaccineStatusBloom Implementation -------------*/

/*---------- Query popStatusByAge Implementation -------------*/

int popStatusByAge(VirusTable *vt,LinkedList *l){
    int status=1;
    Date *date1 = malloc(sizeof(Date));
    Date *date2 = malloc(sizeof(Date));
    status = parse_popStatus(l,&date1,&date2);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_popStatusByAge(vt,l,date1,date2);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    if(date1!=NULL){
        free(date1);
        free(date2);
    }
    return status;
}

//Function to parse popStatusByAge
int parse_popStatus(LinkedList *l,Date **d1,Date **d2){
    if(l->size<2 || l->size>5){
        free(*d1);
        free(*d2);
        return -1;
    }

    //Country was not given
    if(l->size==2){
        free(*d1);
        free(*d2);
        *d1=NULL;
        *d2=NULL;
        if(is_virus(List_get_at(l,1))==-1){
            return -1;
        }
        return 1;
    }
    else if(l->size==3){
        free(*d1);
        free(*d2);
        *d1=NULL;
        *d2=NULL;
        if(is_virus(List_get_at(l,2))==-1 || is_country(List_get_at(l,1))==-1){
            return -1;
        }
        return 1;
    }
    else if(l->size==4){
        if(is_virus(List_get_at(l,1))==-1){
            free(*d1);
            free(*d2);
            return -1;
        }
        if(parse_date(List_get_at(l,2),*d1)==-1 || parse_date(List_get_at(l,3),*d2)==-1){
            free(*d1);
            free(*d2);
            return -1;
        }

        return 1;
    }
    else{
        if(is_country(List_get_at(l,1))==-1){
            free(d1);
            free(d2);
            return -1;
        }
        if(is_virus(List_get_at(l,2))==-1){
            free(d1);
            free(d2);
            return -1;
        }
        if(parse_date(List_get_at(l,3),*d1)==-1 || parse_date(List_get_at(l,4),*d2)==-1){
            free(d1);
            free(d2);
            return -1;
        }

        return 1;
    }

}

//Function to execute the popStatusByAge command
int execute_popStatusByAge(VirusTable *vt,LinkedList *l,Date *date1,Date *date2){
    //Country was not given
    if(l->size==2 || l->size==4){
        char *virus_name = List_get_at(l,1);
        VirusListNode *node = search_vtable_node(vt,virus_name);
        if(node==NULL) return -1;
        printf("\n");
        Skiplist_search_in_range(node->vaccinated,node->not_vaccinated,NULL,date1,date2,0);
        printf("\n\n");
    }
    else{
        char *virus_name = List_get_at(l,1);
        char *country = List_get_at(l,2);
        VirusListNode *node = search_vtable_node(vt,virus_name);
        if(node==NULL) return -1;
        printf("\n");
        Skiplist_search_in_range(node->vaccinated,node->not_vaccinated,country,date1,date2,0);
        printf("\n\n");
    }
    return 1;
}

/*---------- Query popStatusByAge Implementation -------------*/

/*---------- Query populationStatus Implementation -------------*/


int populationStatus(VirusTable *vt, LinkedList *l){
    int status=1;
    Date *date1 = malloc(sizeof(Date));
    Date *date2 = malloc(sizeof(Date));
    status = parse_popStatus(l,&date1,&date2);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_populationStatus(vt,l,date1,date2);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    if(date1!=NULL){
        free(date1);
        free(date2);
    }
    return status;
}



 //Function to execute the populationStatus command
int execute_populationStatus(VirusTable *vt,LinkedList *l,Date *date1,Date *date2){
    //Country was not given
    if(l->size==2 || l->size==4){
        char *virus_name = List_get_at(l,1);
        VirusListNode *node = search_vtable_node(vt,virus_name);
        if(node==NULL) return -1;
        printf("\n");
        Skiplist_search_in_range(node->vaccinated,node->not_vaccinated,NULL,date1,date2,1);
        printf("\n\n");
    }
    else{
        char *virus_name = List_get_at(l,1);
        char *country = List_get_at(l,2);
        VirusListNode *node = search_vtable_node(vt,virus_name);
        if(node==NULL) return -1;
        printf("\n");
        Skiplist_search_in_range(node->vaccinated,node->not_vaccinated,country,date1,date2,1);
        printf("\n\n");
    }
    return 1;
}

/*---------- Query populationStatus Implementation -------------*/

/*---------- Query vaccineStatus Implementation -------------*/

int vaccineStatus(VirusTable *vt,LinkedList *l){
    int status=1;
    status = parse_vaccineStatus(l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_vaccineStatus(vt,l);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    return status;
}

int parse_vaccineStatus(LinkedList *l){
    if((l->size!=2) && (l->size!=3)) return -1;
    if(l->size==2){
        if(is_id(List_get_at(l,1))==-1) return -1;
    }
    else if(l->size==3){
        if(is_id(List_get_at(l,1))==-1) return -1;
        if(is_virus(List_get_at(l,2))==-1) return -1;
    }
     
    return 1;
}

int execute_vaccineStatus(VirusTable *vt,LinkedList *l){
     if(l->size==3){
        char *id = List_get_at(l,1);
        char *virus_name = List_get_at(l,2);
        VirusListNode *node = search_vtable_node(vt,virus_name);
        if(node==NULL) {
            return -1;
        }
        printf("\n");
        Record *rec = SkipList_Search(node->vaccinated,id);
       
            if(rec!=NULL && strcmp(rec->CitizenId,id)==0){
                
                printf("VACCINATED ON %d-%d-%d",rec->dateVaccinated->day,rec->dateVaccinated->month,rec->dateVaccinated->year);
            }
            else{
                printf("NOT VACCINATED");
            }
        }
        else if(l->size==2){
            char *id = List_get_at(l,1);
            for(int i=0; i<vt->num_buckets;i++){
                if(vt->table[i]!=NULL){
                    VirusListNode *cur=vt->table[i]->head;
                    if(cur==NULL) {
                      return -1;
                    }
                    Record *rec = SkipList_Search(cur->vaccinated,id);
                    if(rec!=NULL && strcmp(rec->CitizenId,id)==0){
                        printf("%s %s %d-%d-%d\n",rec->virusName,rec->info,rec->dateVaccinated->day,rec->dateVaccinated->month,rec->dateVaccinated->year);
                    }
                    
                    else{
                         rec= SkipList_Search (cur->not_vaccinated,id);
                        if(rec!=NULL && strcmp(rec->CitizenId,id)==0 ){
                            printf("%s %s\n",rec->virusName,rec->info);
                        }
                    }
        
                    cur=cur->next;
                }
                
            }
        }
        printf("\n\n");
    
    return 1;
}
/*---------- Query vaccineStatus Implementation -------------*/



/*---------- Query insertCitizenrecord Implementation -------------*/

int insertCitizenRecord(HashTable *ht,VirusTable *vt,LinkedList *l){
    int status=1;
    Date *date1 = malloc(sizeof(Date));
    status = parse_insertCitizenRecord(l,&date1);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_insertCitizenRecord(ht,vt,l,date1);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    return status;
}

int parse_insertCitizenRecord(LinkedList *l,Date **d1){
    if(l->size!=8 && l->size!=9){
         free(*d1);
         return -1;
    }
    
    //date not given
    if(l->size==8){
        free(*d1);
        *d1=NULL;
        if( is_id(List_get_at(l,1))==-1 || is_first_last_name(List_get_at(l,2))==-1 || is_first_last_name(List_get_at(l,3))==-1 
        ||  is_country(List_get_at(l,4))==-1 || is_digit(List_get_at(l,5))==-1 || is_virus(List_get_at(l,6))==-1 || is_info(List_get_at(l,7))==-1) {
           
            return -1;
        }
    
    }
    else if(l->size==9){
        
        if( is_id(List_get_at(l,1))==-1 || is_first_last_name(List_get_at(l,2))==-1 || is_first_last_name(List_get_at(l,3))==-1 
        ||  is_country(List_get_at(l,4))==-1 || is_digit(List_get_at(l,5))==-1 || is_virus(List_get_at(l,6))==-1 || is_info(List_get_at(l,7))==-1 || parse_date(List_get_at(l,8),*d1)==-1) {
            
            return -1;
        }
        
    }
    return 1;
}

int execute_insertCitizenRecord(HashTable *ht,VirusTable *vt,LinkedList * l,Date *d1){
    char* id=List_get_at(l,1);
    char* firstname=List_get_at(l,2);
    char* lastname=List_get_at(l,3);
    char* country=List_get_at(l,4);
    int age=atoi(List_get_at(l,5));
    char* virus_name=List_get_at(l,6);
    char* info=List_get_at(l,7);
    
    Record *rec=malloc(sizeof(Record));
    rec->CitizenId=strdup(id);
    rec->virusName=strdup(virus_name);
    rec->firstname=strdup(firstname);
    rec->lastname=strdup(lastname);
    rec->Country=strdup(country);
    rec->age=age;
    rec->info=strdup(info);
    rec->dateVaccinated=d1;

    int flag;
    ht=insert_table(ht,&rec,&flag); 
    if(rec!=NULL && flag==0) {
        vt=insert_vtable(vt,rec);
    }
    else if( flag==1){
        VirusListNode *node = search_vtable_node(vt,rec->virusName);
        Record *oldrec= SkipList_Search(node->not_vaccinated,rec->CitizenId);
        if(rec!=NULL){
            BloomFilter_Insert(node->bloom,oldrec->CitizenId);
            SkipList_Insert(node->vaccinated,oldrec);
            SkipList_Delete(node->not_vaccinated,oldrec->CitizenId);
            free(oldrec->info);
            oldrec->info = strdup("YES");
            oldrec->dateVaccinated=CopyDate(rec->dateVaccinated);
            delRec(rec);
        }

    }

    
    
    return 1;
}

/*---------- Query insertCitizenrecord Implementation -------------*/


/*---------- Query list-nonVaccinated-Persons Implementation -------------*/

int list_non_vaccinated(VirusTable *vt, LinkedList *l){
    int status=1;
    status = parse_list_non(l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_list_non(vt,l);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    return status;
}

int parse_list_non(LinkedList *l){
    if(l->size!=2) return -1;
    if(l->size==2){
        if(is_virus(List_get_at(l,1))==-1) return -1;
    }
    return 1;

}

int execute_list_non(VirusTable *vt, LinkedList *l){
    char *virus_name = List_get_at(l,1);
    VirusListNode *node = search_vtable_node(vt,virus_name);
    if(node==NULL) {
        return -1;
    }
    printf("\n");

    SkipList_Search_list(node->not_vaccinated,virus_name);
 
    return 1;
}

/*---------- Query list-nonVaccinated-Persons Implementation -------------*/



/*---------- Query vaccinateNow Implementation -------------*/

int vaccinateNow(VirusTable *vt,LinkedList *l){
    int status=1;
    status = parse_vaccinateNow(l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status = execute_vaccinateNow(vt,l);
    if(status==-1){
        printf("\nERROR\n\n");
    }
    return status;
}

int parse_vaccinateNow(LinkedList *l){
    if(l->size!=7) return -1;
    if(l->size==7) {
        if( is_id(List_get_at(l,1))==-1 || is_first_last_name(List_get_at(l,2))==-1 || is_first_last_name(List_get_at(l,3))==-1 
        ||  is_country(List_get_at(l,4))==-1 || is_digit(List_get_at(l,5))==-1 || is_virus(List_get_at(l,6))==-1 ) {
            
            return -1;
        }
    }
}

int execute_vaccinateNow(VirusTable *vt, LinkedList *l){
    char* id=List_get_at(l,1);
    char* firstname=List_get_at(l,2);
    char* lastname=List_get_at(l,3);
    char* country=List_get_at(l,4);
    int age=atoi(List_get_at(l,5));
    char* virus_name=List_get_at(l,6);

    VirusListNode *node = search_vtable_node(vt,virus_name);
    if(node==NULL) {
        return -1;
    }
    printf("\n");
    Record *rec = SkipList_Search(node->not_vaccinated,id);
    if(rec!=NULL && strcmp(rec->CitizenId,id)==0){
        BloomFilter_Insert(node->bloom,rec->CitizenId);
        SkipList_Insert(node->vaccinated,rec);
        SkipList_Delete(node->not_vaccinated,rec->CitizenId);
        if(strcmp(rec->info,"NO")==0){
            free(rec->info);
            rec->info=strdup("YES");
            time_t t=time(NULL);
            struct tm date=*localtime(&t);
            rec->dateVaccinated=malloc(sizeof(Date));
            rec->dateVaccinated->day=date.tm_mday;
            rec->dateVaccinated->month=date.tm_mon+1;
            rec->dateVaccinated->year=date.tm_year+1900;
        }
        printf("DONE\n");
        
        printf("%s %s %s %s %d %s %s %d-%d-%d\n\n",rec->CitizenId,rec->firstname,rec->lastname,rec->Country,rec->age,rec->virusName,rec->info,rec->dateVaccinated->day,rec->dateVaccinated->month,rec->dateVaccinated->year);
        
    }
    else{
        rec=SkipList_Search(node->vaccinated,id);
         if(rec!=NULL && strcmp(rec->CitizenId,id)==0 ){
            printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %d-%d-%d\n\n",rec->CitizenId,rec->dateVaccinated->day,rec->dateVaccinated->month,rec->dateVaccinated->year);
        }
    }
    return 1;

}
/*---------- Query vaccinateNow Implementation -------------*/
