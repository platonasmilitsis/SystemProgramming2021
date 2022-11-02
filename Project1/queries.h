#ifndef QUERIES_H
#define QUERIES_H
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "SkipList.h"
#include "BloomFilter.h"
#include "LinkedList.h"


int menu(HashTable *ht,VirusTable *vt);

int popStatusByAge(VirusTable *vt,LinkedList *l);
int parse_popStatus(LinkedList *l,Date **d1,Date **d2);
int execute_popStatusByAge(VirusTable *vt,LinkedList *l,Date *date1,Date *date2);

int populationStatus(VirusTable *vt, LinkedList *l);
int execute_populationStatus(VirusTable *vt,LinkedList *l,Date *date1,Date *date2);

int vaccineStatusBloom(VirusTable *vt,LinkedList *l);
int parse_bloom(LinkedList *l);
int execute_bloom(VirusTable *vt,LinkedList *l);

int vaccineStatus(VirusTable *vt,LinkedList *l);
int parse_vaccineStatus(LinkedList *l);
int execute_vaccineStatus(VirusTable *vt,LinkedList *l);

int insertCitizenRecord(HashTable *ht,VirusTable *vt,LinkedList *l);
int parse_insertCitizenRecord(LinkedList *l,Date **d1);
int execute_insertCitizenRecord(HashTable *ht,VirusTable *vt,LinkedList * l,Date *d1);

int list_non_vaccinated(VirusTable *vt, LinkedList *l);
int parse_list_non(LinkedList *l);
int execute_list_non(VirusTable *vt, LinkedList *l);

int vaccinateNow(VirusTable *vt,LinkedList *l);
int parse_vaccinateNow(LinkedList *l);
int execute_vaccinateNow(VirusTable *vt,LinkedList *l);

#endif //QUERIES_H