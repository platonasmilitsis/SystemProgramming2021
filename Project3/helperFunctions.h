#ifndef HELPERFUNCTIONS
#define HELPERFUNCTIONS

#include "processList.h"
#include "bstlist.h"

//Function to initialize command line arguments
int initializeArguments(int argc,char *argv[],int *numMonitors,int *bufferSize,int *sizeOfBloom,char **input_dir);

//Function to create named pipes
char **create_fifo_pipes(int numMonitors);

//Function for round robin directories
char **round_robin_dirs(int numMonitors,char *input_dir,pid_t *pids,ProcessList *plist);

//Create a log file before exiting
void create_travel_log_file(ProcessList *plist,BstList *bst_l);

void initServerArgs(int argc,char **argv,int *port,int *threads,int *socketBufferSize,int *cyclicBufferSize,int *sizeOfBloom,StringList **paths);

#endif //HELPERFUNCTIONS