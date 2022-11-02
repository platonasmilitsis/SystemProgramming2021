#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "monitorFunctions.h"
#include "parser.h"
#include "pipe_functions.h"
#include "bstlist.h"

//Function to get countries from parent process and save them into a list
StringList *get_countries_from_string(char *msg){
    StringList *countries = StringList_create();
    char *token;
    token=strtok(msg," ");
    while(token!=NULL){
        countries = StringList_Insert(countries,token);
        token = strtok(NULL," ");
    }
    return countries;
}

//Load the structs from the directories
void parse_all_dirs(StringList *countries,StringList *files,char *input_dir,VirusTable **vt,HashTable **ht){
    char *path;
    StringNode *node = countries->head;
    while(node!=NULL){
        //Form the path to the directory file
        path=malloc(strlen(node->value)+strlen(input_dir)+2);
        strcpy(path,input_dir);
        strcat(path,"/");
        strcat(path,node->value);
        struct dirent *de;  
   
        DIR *dr = opendir(path);
        while ((de = readdir(dr)) != NULL){
            if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
                files = StringList_Insert(files,de->d_name);
                char *filepath=malloc(strlen(path)+strlen(de->d_name)+2);
                strcpy(filepath,path);
                strcat(filepath,"/");
                strcat(filepath,de->d_name);
                //Use parser to get records from the given file
                parser(filepath,*ht,*vt);
                free(filepath);
            }
        }
        closedir(dr);
        free(path);
        node=node->next;
    }
}

//Check for new files in the subdirectories and update the structs
void update_files_and_structs(char *input_dir,HashTable **ht,VirusTable **vt,StringList *files,StringList *countries,int rfd,int wfd,int buffer_size){
    char *path;
    StringNode *node = countries->head;
    while(node!=NULL){
        //Form the path to the directory file
        path=malloc(strlen(node->value)+strlen(input_dir)+2);
        strcpy(path,input_dir);
        strcat(path,"/");
        strcat(path,node->value);
        struct dirent *de;  
   
        DIR *dr = opendir(path);
        while ((de = readdir(dr)) != NULL){
            if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
                //File does not exist in list with files
                //Update the structs
                if(StringList_Search(files,de->d_name)==0){
                    files = StringList_Insert(files,de->d_name);
                    char *filepath=malloc(strlen(path)+strlen(de->d_name)+2);
                    strcpy(filepath,path);
                    strcat(filepath,"/");
                    strcat(filepath,de->d_name);
                    //Use parser to get records from the given file
                    parser(filepath,*ht,*vt);
                    free(filepath);
                }
            }
        }
        closedir(dr);
        free(path);
        node=node->next;
    }


    //Get the new bloom filters and send them to the parent process
    char *blooms = package_bloomfilters_vtable(*vt);
    write_to_pipe(wfd,blooms,buffer_size);
    free(blooms);
}

//Create a log file before exiting
void create_monitor_log_file(StringList *slist,BstList *bst_l){

    char *countries = StringList_to_string(slist);
    int accepted=0;
    int declined=0;
    BstList_Request_Sum_Simple(bst_l,&accepted,&declined);
    char buffer[256]; memset(buffer,0,256);
    sprintf(buffer,"\nTOTAL TRAVEL REQUESTS %d\nACCEPTED %d\nREJECTED %d",accepted+declined,accepted,declined);

    //Create and open the log file
    FILE *fp;
    char filename[64]; memset(filename,0,64);
    sprintf(filename,"log_file.%d",getpid());
    fp = fopen (filename, "w+");

    fprintf(fp, "%s", countries);
    fprintf(fp, "%s", buffer);

    free(countries);
    fclose(fp);
    return;
}