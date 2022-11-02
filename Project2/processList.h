#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include "BloomList.h"
#include "StringList.h"
#include <unistd.h>

typedef struct process_list_node{
  pid_t pid;
  BloomList *blist;
  StringList *slist;
  struct process_list_node *next;
}ProcessListNode;

typedef struct processList{
    int size;
    ProcessListNode *head;
    ProcessListNode *tail;
}ProcessList;

ProcessList *ProcessList_create();
ProcessListNode *Create_processList_node(pid_t prid);
ProcessList *ProcessList_Insert(ProcessList *PList, pid_t prid);
int ProcessList_Search(ProcessList *PList,char *country);
ProcessList *ProcessList_add_country(ProcessList *PList,pid_t prid,char *country);
ProcessList *ProcessList_add_bloomlist(ProcessList *PList,pid_t prid,BloomList *blist);
int ProcessList_search_blooms(ProcessList *PList,char *id,char *country,char *virus);
void ProcessList_print(ProcessList *PList);
char * ProcessList_to_string(ProcessList *PList);
int ProcessList_Send_Signal_To_ALL(ProcessList *PList,int signo);
void ProcessList_Destroy(ProcessList *PList);

#endif //PROCESSLIST_H