#ifndef MONITORFUNCTIONS_H
#define MONITORFUNCTIONS_H

#include "StringList.h"
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "bstlist.h"

//Function to get countries from parent process and save them into a list
StringList *get_countries_from_string(char *msg);

//Load the structs from the directories
void parse_all_dirs(StringList *countries,StringList *files,char *input_dir,VirusTable **vt,HashTable **ht);

//Check for new files in the subdirectories and update the structs
void update_files_and_structs(char *input_dir,HashTable **ht,VirusTable **vt,StringList *files,StringList *countries,int rfd,int wfd,int buffer_size);

//Create a log file before exiting
void create_monitor_log_file(StringList *countries,BstList *bst_l);

#endif //MONITORFUNCTIONS_H