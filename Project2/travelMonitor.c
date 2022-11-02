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

#include "helperFunctions.h"
#include "pipe_functions.h"
#include "BloomList.h"
#include "processList.h"
#include "travelMonitorQueries.h"
#include "bstlist.h"

int main(int argc, char **argv){
    
	char* input_dir = NULL;
	int numMonitors, bufferSize,sizeOfBloom;

	//Check the validity and init command line arguments
	if(initializeArguments(argc,argv,&numMonitors, &bufferSize,&sizeOfBloom,&input_dir)==-1){
		return -1;
	}

	//Create filenames and pipes
	//Fd array
	int *fds = malloc(sizeof(int)*(numMonitors*2));
	char **named_pipes = create_fifo_pipes(numMonitors);
	pid_t *pids = malloc(sizeof(pid_t)*(numMonitors));
	//Create a list containing information for each process
	ProcessList *plist = ProcessList_create();

	//Create child processes and exec them
	//parent process will open pipes on its end
	pid_t pid;
	for(int i=0;i<numMonitors;i++){
		pid = fork();
		//Chlid process
		if(pid==0){
			char buffer_string[12];
			char bloom_string[12];
			memset(buffer_string,0,12);
			sprintf(buffer_string,"%d",bufferSize);
			memset(bloom_string,0,12);
			sprintf(bloom_string,"%d",sizeOfBloom);

			char *args[]={"./Monitor",named_pipes[2*i],named_pipes[2*i+1],input_dir,buffer_string,bloom_string,NULL};

            if(execv(args[0],args)==-1){
                perror("Execv error");
                exit(127);
            }

		}
		//pid is not 0 -> this is the parent process
		else{
			pids[i] = pid;
			plist = ProcessList_Insert(plist,pid);
            //Open the files for the pipes
            fds[2*i]=open(named_pipes[2*i], O_WRONLY);
            if(fds[2*i]<1){
                perror("Open error");
                return 1;
            }
            fds[2*i+1]=open(named_pipes[2*i+1], O_RDONLY);
            if(fds[2*i+1]<1){
                perror("Open error");
                return 1;
            }
		}
	}

	//Pass the country dirs to every Monitor process
	char **msgs = round_robin_dirs(numMonitors,input_dir,pids,plist);
	for(int i=0;i<numMonitors;i++){
		write_to_pipe(fds[2*i],msgs[i],bufferSize);
		free(msgs[i]);
	}
	free(msgs);

	//Get the bloom filters from every child
	char **blooms = malloc(sizeof(char*)*numMonitors);
	for(int i=0;i<numMonitors;i++){
		blooms[i]=read_from_pipe(fds[2*i+1],bufferSize);
		char *disease=NULL;
		BloomList *blist = BloomList_Unpack(blooms[i],sizeOfBloom,16);
		plist = ProcessList_add_bloomlist(plist,pids[i],blist);
		free(blooms[i]);
	}
	free(blooms);


	//perform user queries
	BstList *bst_l = BstList_create();
	travel_menu(fds,pids,numMonitors,bufferSize,sizeOfBloom,plist,bst_l);

    int status;
    while(waitpid(-1, &status, 0) > 0);

	//Create a log file before exiting
	create_travel_log_file(plist,bst_l);

	for(int i=0;i<numMonitors*2;i++){
		close(fds[i]);
		unlink(named_pipes[i]);
		free(named_pipes[i]);
	}
	free(named_pipes);
	free(fds);
	free(pids);
	free(input_dir);
	BstList_Destroy(bst_l);
	ProcessList_Destroy(plist);
    return 0;
}
