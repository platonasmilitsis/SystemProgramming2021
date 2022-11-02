#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/wait.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h>  
#include <stdlib.h> 
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#include "helpMonitorClient.h"
#include "pipe_functions.h"
#include "helperFunctions.h"
#include "processList.h"
#include "travelMonitorQueries.h"

int main(int argc, char **argv){

    char* input_dir=NULL;
    int numMonitors,sizeOfBloom,numThreads,socketBufferSize,cyclicBufferSize,numPaths;


    if(inArguments(argc,argv,&numMonitors,&socketBufferSize,&cyclicBufferSize,&sizeOfBloom,&input_dir,&numThreads)==-1){
        return -1;
    }

    int *pids = malloc(sizeof(int)*numMonitors);
    //init the ports
    srand(time(NULL));
    int *ports = initPorts(numMonitors,rand()%45000);
    //init the port list
    ProcessList *plist = ProcessList_create();
    for(int i=0;i<numMonitors;i++)
        ProcessList_Insert(plist,ports[i]);
    char **dirs = round_robin_dirs(numMonitors,input_dir,ports,plist);
    

    for(int i=0;i<numMonitors;i++){
        char **args = initServerArguments(ports[i],numThreads,
            socketBufferSize,cyclicBufferSize,sizeOfBloom,dirs[i]);
        pid_t pid;

        pid = fork();
        //Chlid process
        if(pid==0){
            

            if(execv(args[0],args)==-1){
                perror("Execv error");
                exit(127);
            }

        }
        else
            pids[i] = pid;
    }

    int *sockets_fd = malloc(sizeof(int) * numMonitors);
    char **boom = malloc(sizeof(char*)*numMonitors);

    for(int i=0;i<numMonitors;i++){
        char host[256];
        char *serverIp;
        struct hostent *host_entry;
        int hostname;
        hostname = gethostname(host, sizeof(host)); //find the host name
        host_entry = gethostbyname(host); //find host information
        serverIp = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        struct sockaddr *serverptr = (struct sockaddr*)&serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror_exit("\n Socket creation error \n");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(ports[i]);
        
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, serverIp, &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }


        // int Statusock;
        // do{
        //     Statusock=connect(sock,serverptr,sizeof(serv_addr));
        // }while(Statusock<0);
        while(connect(sock, serverptr, sizeof(serv_addr))<0){
        }

        sockets_fd[i] = sock;

        printf("Connected\n");
        //write_to_pipe(sock,dirs[i],socketBufferSize);
        boom[i] = read_from_pipe(sock,socketBufferSize);
        BloomList *blist = BloomList_Unpack(boom[i],sizeOfBloom,16);
		plist = ProcessList_add_bloomlist(plist,ports[i],blist);
		free(boom[i]);
       // free(dirs[i]);
        close(sock);
    }

    
    BstList *bst_l=BstList_create();
    travel_menu(sockets_fd,ports,numMonitors,socketBufferSize,sizeOfBloom,plist,bst_l);

    
    int status;
    while(waitpid(-1, &status, 0) > 0);
   
   //Create a log file before exiting
	create_travel_log_file(plist,bst_l);
   
    free(sockets_fd);
    free(input_dir);
    free(pids);
    free(ports);
    free(boom);
    free(dirs);
    ProcessList_Destroy(plist);
    BstList_Destroy(bst_l);


    return 0;
}