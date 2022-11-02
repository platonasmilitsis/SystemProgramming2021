#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include "helperFunctions.h"
#include "bstlist.h"

//Function to initialize command line arguments
int initializeArguments(int argc,char *argv[],int *numMonitors,int *bufferSize,int *sizeOfBloom,char **input_dir){
	if (argc != 9){								
		printf("Invalid number of args, exiting\n");
		return -1;
	}

    for (int i = 1; i <= 7 ; i += 2 ){			
		if (!strcmp(argv[i], "-m" )){			
			*numMonitors = atoi(argv[i + 1]);
		}
		else if(!strcmp(argv[i], "-b")){
			*bufferSize = atoi(argv[i + 1]);
		}
        else if(!strcmp(argv[i], "-s")){
            *sizeOfBloom = atoi(argv[i + 1]); 
        }
		else if(!strcmp(argv[i], "-i")){
			*input_dir = strdup(argv[i + 1]);
		}
		else{
			if(input_dir != NULL){
				free(*input_dir);
			}
			printf("Error in arguments (flags), exiting\n");
			return -1;
		}
	}

    return 1;
}

//Function to create named pipes
char **create_fifo_pipes(int numMonitors){
    char **named_pipes = malloc(sizeof(char*)*numMonitors*2);
    char name_buffer[128];
    for(int i=0;i<numMonitors*2;i++){
        memset(name_buffer,0,128);
        sprintf(name_buffer,"nakos.%d",i);
        named_pipes[i] = strdup(name_buffer);
        if(mkfifo(named_pipes[i],0666)<0 && (errno!=EEXIST)){
            printf("%s\n",named_pipes[i]);
            perror("cannot create fifo");
            for(int i=0;i<numMonitors*2;i++){
                if(named_pipes[i]!=NULL)
                    free(named_pipes[i]);
            }
            free(named_pipes);
            return NULL;
        }
    }
    return named_pipes;
}

//Function for round robin directories
char **round_robin_dirs(int numMonitors,char *input_dir,pid_t *pids,ProcessList *plist){
    struct dirent *de;  
    struct dirent* pDirent;
   
    DIR *dr = opendir(input_dir);

    //Create array with messages for the country dirs
    char **msgs = malloc(sizeof(char*)*numMonitors);
    for(int i=0;i<numMonitors;i++) msgs[i]=NULL;
  
    if (dr == NULL)  
    {
        printf("Could not open current directory" );
        return 0;
    }

    int robin_index=0;
    while ((de = readdir(dr)) != NULL){
        if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
            plist = ProcessList_add_country(plist,pids[robin_index],de->d_name);
            if(msgs[robin_index]==NULL){
                msgs[robin_index]=malloc(strlen(de->d_name)+2);
                strcpy(msgs[robin_index],de->d_name);
                strcat(msgs[robin_index]," ");
            }
            else{
                msgs[robin_index]=realloc(msgs[robin_index],strlen(msgs[robin_index])+strlen(de->d_name)+2);
                strcat(msgs[robin_index],de->d_name);
                strcat(msgs[robin_index]," ");
            }
            robin_index++;
            if(robin_index==numMonitors)
                robin_index=0;
        }
    }
  
    closedir(dr);    
    return msgs;
}

//Create a log file before exiting
void create_travel_log_file(ProcessList *plist,BstList *bst_l){

    char *countries = ProcessList_to_string(plist);
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