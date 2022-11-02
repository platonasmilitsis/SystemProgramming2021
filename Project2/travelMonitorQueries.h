#ifndef TRAVELMONITORQUERIES_H
#define TRAVELMONITORQUERIES_H

#include "BloomList.h"
#include "StringList.h"
#include "processList.h"
#include "pipe_functions.h"
#include "LinkedList.h"
#include "bstlist.h"

void travel_menu(int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomSize,ProcessList *plist,BstList *bst_l);

int travelRequest(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l);
int execute_travelRequest(ProcessList *plist,LinkedList *l,Date *d1,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l);
int parse_travel_request(LinkedList *l,Date *d1);

int travelStats(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l);
int execute_travelStats(ProcessList *plist,LinkedList *l,Date *date1,Date *date2,int *fds,pid_t *pids,int numMonitors,int bufferSize,BstList *bst_l);
int parse_travelStats(LinkedList *l,Date **d1,Date **d2);

int addvaccinationRecords(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomsize);
int execute_addvaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize,int bloomsize);
int parse_addvaccination(LinkedList *l);

int searchVaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize);
int execute_searchVaccination(ProcessList *plist,LinkedList *l,int *fds,pid_t *pids,int numMonitors,int bufferSize);
int parse_searchVaccination(LinkedList *l);


#endif //TRAVELMONITORQUERIES_H