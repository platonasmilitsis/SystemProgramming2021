#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VirusTable.h"
#include "HashTable.h"
#include "Record.h"
#include "LinkedList.h"
#include "parser.h"
#include "queries.h"
#include "SkipList.h"
#include "BloomFilter.h"


//function to parse the records from citizenrecordsfile
int parser(char * filename, HashTable *ht, VirusTable *vt){

    FILE *fp=fopen(filename,"r");

     if(!fp){
          printf("File Not Found\n");
          return -1;
      }

    
    char *line = NULL;
    size_t length = 0;
    
     while(getline(&line, &length, fp) != -1) {

         
    
        Record *r=createRec(line);
        int flag;
        ht=insert_table(ht,&r,&flag); 
        if(r!=NULL && flag==0) {
            vt=insert_vtable(vt,r);
        }
        else if( flag==1){
            VirusListNode *node = search_vtable_node(vt,r->virusName);
            Record *rec= SkipList_Search(node->not_vaccinated,r->CitizenId);
            if(rec!=NULL){
               BloomFilter_Insert(node->bloom,rec->CitizenId);
               SkipList_Insert(node->vaccinated,rec);
               SkipList_Delete(node->not_vaccinated,rec->CitizenId);
               free(rec->info);
               rec->info = strdup("YES");
               rec->dateVaccinated=CopyDate(r->dateVaccinated);
               delRec(r);
            }

        }

     }
    
   free(line);
   fclose(fp); 
   return 1;
}

//Function to break a command into tokens
LinkedList *tokenize_input(char *command){
   LinkedList *l = List_create();
   char *token = strtok(command," ");
   while(token!=NULL){
      l = List_Insert(l,token);
      token = strtok(NULL," ");
   }
   return l;
}

//Function to connect tokens into a command
char *format_command(LinkedList *l){
   char *command;
   int size=0;
   Listnode *node = l->head;
   while(node!=NULL){
      size += strlen(node->data)+1;
      node = node->next;
   }
   command = malloc(size+1);
   memset(command,0,size+1);

   Listnode *cur = l->head;
   while(cur!=NULL){
      strcat(command,cur->data);
      strcat(command," ");
      cur = cur->next;
   }
   return command;
}

//Function to check if a token is a proper id 
int is_id(char* token){
    for(int i=0;i<strlen(token);i++){
      if(token[i]<'0' || token[i]>'9')
         return -1;
   }
   return 1;
}

int is_info(char *token){
   if(strcmp(token,"YES")!=0 && strcmp(token,"NO")!=0) return-1;
   return 1;
}

//Function to check if a token is a proper virus name
int is_virus(char *token){
   for(int i=0;i<strlen(token);i++){
      if((token[i]<'0' || token[i]>'9') && (token[i]<'a' || token[i]>'z') && (token[i]<'A' || token[i]>'Z') && token[i]!='-')
         return -1;
   }
   return 1;
}

int is_country(char *token){
   for(int i=0;i<strlen(token);i++){
      if((token[i]<'a' || token[i]>'z') && (token[i]<'A' || token[i]>'Z'))
         return -1;
   }
   return 1;
}
int is_first_last_name(char *token){
   for(int i=0;i<strlen(token);i++){
      if((token[i]<'a' || token[i]>'z') && (token[i]<'A' || token[i]>'Z'))
         return -1;
   }
   return 1;
}

int is_digit(char *token){
   for(int i=0;i<strlen(token);i++){
      if(token[i]<'0' || token[i]>'9')
         return -1;
   }
   return 1;
}

//Function to parse a given date
int parse_date(char *token,Date *d){
   char *date = strdup(token);
   char *temp = strtok(date,"-");
   if(temp==NULL || is_digit(temp)==-1){free(date); return -1;}
   d->day = strtol(temp,NULL,10);
   temp = strtok(NULL,"-");
   if(temp==NULL || is_digit(temp)==-1){free(date); return -1;}
   d->month = strtol(temp,NULL,10);
   temp = strtok(NULL,"-");
   if(temp==NULL || is_digit(temp)==-1){free(date); return -1;}
   d->year = strtol(temp,NULL,10);
   free(date);
   return 1;
}


int initargs(int argc,char** argv,char **filename,int * bloomsize){
  
   if(argc != 5 ){
      return -1;
   }
   if(strcmp(argv[1],"-c")==0){
      *filename=strdup(argv[2]);
   }
   else if(strcmp(argv[1],"-b")==0){
      *bloomsize=atoi(argv[2]);
   }
   if(strcmp(argv[3],"-c")==0){
   *filename=strdup(argv[4]);
   }
   else if(strcmp(argv[3],"-b")==0){
      *bloomsize=atoi(argv[4]);
   }
   else{
      return -1;
   }
   
   return 1;
}