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
#include "pipe_functions.h"
#include "monitorFunctions.h"
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "bstlist.h"
#include "queries.h"

int sigkill_flag=0;
int sigusr2_flag=0;
int sigusr1_flag=0;

void signal_handler(int signo){
    if(signo==SIGINT || signo==SIGQUIT){
        sigkill_flag=1;
    }
    else if(signo==SIGUSR2){
        sigusr2_flag=1;
    }
    else if(signo==SIGUSR1){
        sigusr1_flag=1;
    }
}

int main(int argc, char **argv){

    //Setup the signal handling
    //set the handler for SIGINT and SIGQUIT
    static struct sigaction act;
    act.sa_handler=signal_handler;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    //set handler for SIGUSR2
    static struct sigaction act_usr2;
    act_usr2.sa_handler=signal_handler;
    sigfillset(&(act_usr2.sa_mask));
    sigaction(SIGUSR2, &act_usr2, NULL);

    //set handler for SIGUSR1
    static struct sigaction act_usr1;
    act_usr1.sa_handler=signal_handler;
    sigfillset(&(act_usr1.sa_mask));
    sigaction(SIGUSR1, &act_usr1, NULL);


    char *reader_pipe = strdup(argv[1]);
    char *writer_pipe = strdup(argv[2]);
    char *input_dir = strdup(argv[3]);
    int buffer_size = atoi(argv[4]);
    int sizeOfbloom = atoi(argv[5]);

    int wfd;
    int rfd;

    //Open the pipes for reading and writing
    rfd=open(reader_pipe, O_RDONLY);
    wfd=open(writer_pipe, O_WRONLY);

    //Read the directories from the parent process
    char *msg = read_from_pipe(rfd, buffer_size);
    //Save the countries into a list
    StringList *countries = get_countries_from_string(msg);
    free(msg);

    //Create the structs for the queries
    VirusTable *vt=create_vtable(MAX_SIZE,sizeOfbloom,16);
    HashTable *ht=create_table(MAX_SIZE);
    BstList *bst_l = BstList_create();
    //Load the structs from the directories
    StringList *files = StringList_create();
    parse_all_dirs(countries,files,input_dir,&vt,&ht);

    //Send the bloom filters to the travelMonitor
    char *blooms = package_bloomfilters_vtable(vt);
    write_to_pipe(wfd,blooms,buffer_size);
    free(blooms);
    
    sigset_t int_set;
    sigemptyset(&int_set);
    while(1){

        if(sigkill_flag==1){
            break;
        }

        //New file came into this monitor's directories
        if(sigusr1_flag==1){
            update_files_and_structs(input_dir,&ht,&vt,files,countries,rfd,wfd,buffer_size);
            sigusr1_flag=0;
        }

        if(sigusr2_flag==1){
            sigset_t critical_set;
            sigfillset(&critical_set);
            sigprocmask(SIG_SETMASK, &critical_set, NULL);
            monitor_commands(ht,vt,bst_l,rfd,wfd,buffer_size);
            sigusr2_flag=0;
            sigprocmask(SIG_UNBLOCK, &critical_set, NULL);
        }

        sigsuspend(&int_set);

        if(sigkill_flag==1){
            break;
        }

        //New file came into this monitor's directories
        if(sigusr1_flag==1){
            update_files_and_structs(input_dir,&ht,&vt,files,countries,rfd,wfd,buffer_size);
            sigusr1_flag=0;
        }

        if(sigusr2_flag==1){
            sigset_t critical_set;
            sigfillset(&critical_set);
            sigprocmask(SIG_SETMASK, &critical_set, NULL);
            monitor_commands(ht,vt,bst_l,rfd,wfd,buffer_size);
            sigusr2_flag=0;
            sigprocmask(SIG_UNBLOCK, &critical_set, NULL);
        }

    }


    create_monitor_log_file(countries,bst_l);

    close(rfd);
    close(wfd);
    StringList_Destroy(countries);
    StringList_Destroy(files);
    delete_table(ht);
    delete_vtable(vt);
    BstList_Destroy(bst_l);
    free(input_dir);
    free(reader_pipe);
    free(writer_pipe);
    return 0;
}