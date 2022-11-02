#ifndef PARSER_H
#define PARSER_H

#include "LinkedList.h"
#include "VirusTable.h"
#include "HashTable.h"

int parser(char * filename, HashTable *ht, VirusTable *vt);
LinkedList *tokenize_input(char *command);
int is_id(char* token);
int is_first_last_name(char *token);
int is_virus(char *token);
int is_digit(char *token);
int is_country(char *token);
int parse_date(char *token,Date *d);
int is_info(char *token);
int initargs(int argc,char** argv,char **filename,int * bloosize);



#endif //PARSER_H
