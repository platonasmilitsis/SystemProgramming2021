#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h>  
#include <stdlib.h> 
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "helpMonitorClient.h"
#include "pipe_functions.h"
#include "helperFunctions.h"
#include "pipe_functions.h"
#include "monitorFunctions.h"
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "bstlist.h"
#include "Threads.h"
#include "queries.h"
#include "travelMonitorQueries.h"
#include "queries.h"

int main(int argc, char **argv){

    int port,threads,socketBufferSize,cyclicBufferSize,sizeOfBloom;
    StringList *paths=StringList_create();

    initServerArgs(argc,argv,&port,&threads,&socketBufferSize,&cyclicBufferSize,&sizeOfBloom,&paths);


	printf("Server Started %d\n",getpid());

	char host[256];
	char *IP;
	struct hostent *host_entry;
	int hostname;
	hostname = gethostname(host, sizeof(host)); //find the host name
	// check_host_name(hostname);
	host_entry = gethostbyname(host); //find host information
	// check_host_entry(host_entry);
	IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string



	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};


    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror_exit("socket failed");
       
    }
       
    // Forcefully attaching socket to the port 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror_exit("setsockopt");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[2]));
    
       
    // Forcefully attaching socket to the port 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror_exit("bind failed");
    }

    
    if (listen(server_fd,128) < 0){
        perror_exit("listen");
    }
    

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror_exit("accept");
    }
    
    //Create the structs for the queries
    VirusTable *vt=create_vtable(MAX_SIZE,sizeOfBloom,16);
    HashTable *ht=create_table(MAX_SIZE);
    BstList *bst_l = BstList_create();
    //Load the structs from the directories
    StringList *files = StringList_create();

    pthread_mutex_t *locking_queue;
    pthread_mutex_t *locking_structs;
    pthread_cond_t *nonempty;
    pthread_cond_t *nonfull;
    pthread_cond_t *structs_filled;

    int countriesCounter = paths->size;
    
    locking_queue = malloc(sizeof(pthread_mutex_t));
    locking_structs = malloc(sizeof(pthread_mutex_t));

    pthread_mutex_init(locking_queue,NULL);
    pthread_mutex_init(locking_structs,NULL);

    nonempty = malloc(sizeof(pthread_cond_t));
	nonfull = malloc(sizeof(pthread_cond_t));
    structs_filled = malloc(sizeof(pthread_cond_t));

    pthread_cond_init(nonempty,NULL);
    pthread_cond_init(nonfull,NULL);
    pthread_cond_init(structs_filled,NULL);
	
    cyclicBuffer *cb = cb_init(cyclicBufferSize,locking_queue, nonempty, nonfull); 
    threadArgs *args = argsInit(cb, vt, ht,files,locking_queue,nonempty, nonfull,locking_structs,structs_filled,&countriesCounter);


    pthread_t *thr = malloc(sizeof(pthread_t)*2);


    for(int i=0;i<threads;i++){
        pthread_create(&thr[i],NULL, thread_fun,(void*)args);
    }

    while(paths->head!=NULL){
        char *path = StringList_popPath(paths, "dir");
        //printf("%s\n",path);
        cb = cb_place(cb,path);
    }
    

    pthread_mutex_lock(locking_structs);
    while(countriesCounter!=0){
        pthread_cond_wait(structs_filled, locking_structs);
    }


    int newsock;

	// fd_set descriptors;
	// FD_ZERO(&descriptors);
	// FD_SET(server_fd,&descriptors);
	// int n = select(server_fd+1,&descriptors,NULL,NULL,NULL);
	int newclientlen = sizeof(address);
	// if(newsock=accept(server_fd,(struct sockaddr*) &address,&newclientlen)<0){
	// 	perror_exit("accept");
    // }
	
    //Send the bloom filters to the travelMonitorClient
    // char *dir=read_from_pipe(new_socket,socketBufferSize);
    // StringList *countries=get_countries_from_string(dir);
    char *blooms = package_bloomfilters_vtable(vt);
	write_to_pipe(new_socket,blooms,socketBufferSize);
    free(blooms);
    //free(dir);
    close(new_socket);



    printf("Server Port : %d\n",port);
    while(1){
        if((newsock=accept(server_fd,(struct sockaddr*) &address,&newclientlen))<0){
            perror_exit("accept");
        }
        if(monitor_commands(ht,vt,bst_l,newsock,socketBufferSize)==0){
            close(newsock);
            break;
        }

        close(newsock);
    }

    //Create a log file before exiting
    //create_monitor_log_file(countries,bst_l);

    delete_vtable(vt);
    delete_table(ht);
    BstList_Destroy(bst_l);
    StringList_Destroy(paths);
    StringList_Destroy(files);

    return 0;
}
	
   