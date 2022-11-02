#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>

#include "travelMonitorQueries.h"
#include "parser.h"
#include "BloomFilter.h"
#include "LinkedList.h"
#include "bstlist.h"

int sigint_menu;

void menu_handler(int signo){
    if(signo==SIGINT || signo==SIGQUIT){
        sigint_menu=1;
    }
}


void travel_menu(int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomSize,ProcessList *plist,BstList *bst_l){
    char *line = NULL;
    size_t len = 0;

    //Setup the signal handling
    //set the handler for SIGINT and SIGQUIT
    static struct sigaction act;
    act.sa_handler=menu_handler;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    printf("Insert Commands:\n\n");

    while(1){

        //SIGINT case
        if(sigint_menu==1){
            ProcessList_Send_Signal_To_ALL(plist,SIGKILL);
            free(line);
        }

        getline(&line, &len, stdin);

        //SIGINT case
        if(sigint_menu==1){
            ProcessList_Send_Signal_To_ALL(plist,SIGKILL);
            free(line);
        }

        if(strcmp(line,"\n")==0) continue;
        //Remove change line characters
        line = strtok(line,"\n\r");
        LinkedList *l = tokenize_input(line);

        //Get the command type
        char *type = List_get_at(l,0);

        if(strcmp(type,"/exit")==0){
            ProcessList_Send_Signal_To_ALL(plist,SIGINT);
            List_destroy(l);
            free(line);
            return;
        }
        else if(strcmp(type,"/travelRequest")==0){
           travelRequest(plist,l,fds,pids,numMonitors,bufferSize,bst_l);
        }
        else if(strcmp(type,"/travelStats")==0){
            travelStats(plist,l,fds,pids,numMonitors,bufferSize,bst_l);
        }
        else if(strcmp(type,"/addVaccinationRecords")==0){
            addvaccinationRecords(plist,l,fds,pids,numMonitors,bufferSize,bloomSize);
        }
        else if(strcmp(type,"/searchVaccinationStatus")==0){
            searchVaccination(plist,l,fds,pids,numMonitors,bufferSize);
        }
        else{
            printf("\nERROR\n\n");
        }

        List_destroy(l);
    }


    free(line);
}

int travelRequest(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l){
    int status=1;
    Date *d1 = malloc(sizeof(Date));
    status=parse_travel_request(l,d1);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status=execute_travelRequest(plist,l,d1,fds,pids,numMonitors,bufferSize,bst_l);
     if(status==-1){
        printf("\nERROR\n\n");
    }

    return status;
}

int execute_travelRequest(ProcessList *plist,LinkedList *l,Date *date1,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l){

    char *id = List_get_at(l,1);
    char *countryFrom = List_get_at(l,3);
    char *countryTo = List_get_at(l,4);
    char *virus = List_get_at(l,5);

    //Check the bloom filters for the given record
    if(ProcessList_search_blooms(plist,id,countryFrom,virus)==0){
        bst_l = BstList_Insert(bst_l,countryTo,virus,date1,0);
        printf("\nREQUEST REJECTED – YOU ARE NOT VACCINATED\n\n");
        free(date1);
        return 1;
    }

    pid_t pid = ProcessList_Search(plist,countryFrom);
    //Signal the correct Monitor process to execute the query
    kill(pid,SIGUSR2);
    //Feed the input to the monitor process through the pipe
    int index=0;
    for(int i=0;i<numMonitors;i++){
        if(pid==pids[i]){
            index=i;
            break;
        }
    }
    char *command = format_command(l);
    //Write the command into the pipe
    write_to_pipe(fds[2*index],command,bufferSize);
    //Read the Monitor process' response
    char *msg = read_from_pipe(fds[2*index+1],bufferSize);
    if(strcmp(msg,"NO")==0){
        bst_l = BstList_Insert(bst_l,countryTo,virus,date1,0);
        printf("\nREQUEST REJECTED – YOU ARE NOT VACCINATED\n\n");
    }
    else{
        char *string_date = NULL;
        Date *date2 = malloc(sizeof(Date));
        string_date = strtok(msg," ");
        string_date = strtok(NULL," ");
        parse_date(string_date,date2);
        if(Dates_Difference(date2,date1)<180 && Dates_Difference(date2,date1)>-180){
            bst_l = BstList_Insert(bst_l,countryTo,virus,date1,1);
            printf("\nREQUEST ACCEPTED – HAPPY TRAVELS\n\n");
        }
        else{
            bst_l = BstList_Insert(bst_l,countryTo,virus,date1,0);
            printf("\nREQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE\n\n");
        }
        free(date2);
    }    

    free(date1);
    free(msg);
    free(command);
    return 1;
}

int parse_travel_request(LinkedList *l,Date *d1){
    if(l->size!=6){  
        free(d1);
        return -1;
    }

    if(l->size==6){
        if(is_id(List_get_at(l,1))==-1){
            free(d1);
            return -1;
        }
        if(parse_date(List_get_at(l,2),d1) ==-1){
            free(d1);
            return -1;
        }
        if(is_country(List_get_at(l,3))==-1) {
            free(d1);
            return -1;
        }
        if(is_country(List_get_at(l,4))==-1) {
            free(d1);
            return -1;
        }
        if(is_virus(List_get_at(l,5))==-1){
            free(d1);
            return -1;
        }
    }
    return 1;
}

    
int travelStats(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l){
    int status=1;
    Date *d1=malloc (sizeof(Date));
    Date *d2=malloc (sizeof(Date));
    status=parse_travelStats(l,&d1,&d2);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status=execute_travelStats(plist,l,d1,d2,fds,pids,numMonitors,bufferSize,bst_l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    return status;
}

int execute_travelStats(ProcessList *plist,LinkedList *l,Date *date1,Date *date2,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l){
    
    char *virus = List_get_at(l,1);  
    char *country = NULL;    
    //No country was given
    if(l->size==5){
        country =  List_get_at(l,4);    
    }

    int accepted=0;
    int declined=0;
    BstList_Request_Sum(bst_l,NULL,virus,date1,date2,&accepted,&declined);
    printf("\nTOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n\n",accepted+declined,accepted,declined);

    free(date1);
    free(date2);
    return 1;
}



int parse_travelStats(LinkedList *l,Date **d1,Date **d2){
    if(l->size!=4 && l->size!=5) {
        free(*d1);
        free(*d2);
        return -1;
    }

    if(l->size==4){
        if(is_virus(List_get_at(l,1))==-1){
            free(*d1);
            free(*d2);
            return -1;
        } 
    
        if ((parse_date(List_get_at(l,2),*d1)==-1) || (parse_date(List_get_at(l,3),*d2)==-1)){
            free(*d1);
            free(*d2);
            return -1;
        }
    }
    else if(l->size==5){
        if(is_virus(List_get_at(l,1))==-1){
            free(*d1);
            free(*d2);
            return -1;
        } 

        if ((parse_date(List_get_at(l,2),*d1)==-1) || (parse_date(List_get_at(l,3),*d2)==-1)){
            free(*d1);
            free(*d2);
            return -1;
        }
        if(is_country(List_get_at(l,4))==-1) {
            free(*d1);
            free(*d2);
            return -1;
        } 
    }
    return 1;
}




int addvaccinationRecords(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomsize){
    int status=1;
    status=parse_addvaccination(l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status=execute_addvaccination(plist,l,fds,pids,numMonitors,bufferSize,bloomsize);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    return status;
}

int execute_addvaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomsize){

    char *country = List_get_at(l,1);
    pid_t pid = ProcessList_Search(plist,country);
    int index=0;
    for(int i=0;i<numMonitors;i++){
        if(pid==pids[i]){
            index=i;
            break;
        }
    }

    //Send SIGUSR1 to the monitor that keeps the country
    kill(pid,SIGUSR1);

    //Get the updated bloom filters
	//Get the bloom filters from every child
	char *blooms=read_from_pipe(fds[2*index+1],bufferSize);
	BloomList *blist = BloomList_Unpack(blooms,bloomsize,16);
	plist = ProcessList_add_bloomlist(plist,pid,blist);
	free(blooms);

    printf("\nDONE\n\n");

    return 1;
}


int parse_addvaccination(LinkedList *l){
    if(l->size!=2) return -1;
    if(l->size==2){
        if(is_country(List_get_at(l,1))==-1) return -1;
    }
    return 1;
}



int searchVaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize){
    int status=1;
    status=parse_searchVaccination(l);
    if(status==-1) { printf("\nERROR\n\n"); return status;}
    status=execute_searchVaccination(plist,l,fds,pids,numMonitors,bufferSize);
    return status;
}


int execute_searchVaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize){

    //Signal to all processes that they are about to recieve a query
    ProcessList_Send_Signal_To_ALL(plist,SIGUSR2);

    char *command = format_command(l);
    for(int i=0;i<numMonitors;i++){
        write_to_pipe(fds[2*i],command,bufferSize);
    }
    
    //Get the output of all Monitor processes
    char **msgs = malloc(sizeof(char*)*numMonitors);
    for(int i=0;i<numMonitors;i++){
        msgs[i] = read_from_pipe(fds[2*i+1],bufferSize);
        if(strcmp(msgs[i],"None")!=0) printf("\n%s\n",msgs[i]);
        free(msgs[i]);
    }
    free(msgs);

    free(command);
    return 1;
}

int parse_searchVaccination(LinkedList *l){
    if(l->size!=2) return -1;
    if(l->size==2){
        if(is_id(List_get_at(l,1))==-1) return -1;
    }
    return 1;
}

