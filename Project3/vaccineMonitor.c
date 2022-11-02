#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SkipList.h"
#include "HashTable.h"
#include "VirusTable.h"
#include "parser.h"
#include "queries.h"

int main(int argc,char *argv[]){

    char *filename;
    int bloomsize=0;
    int status=0;
    status=initargs(argc,argv,&filename,&bloomsize);
    if(status==-1){
        printf("WRONG ARGUMENTS\n");
        return 1;
    }
    VirusTable *vt=create_vtable(MAX_SIZE,bloomsize,16);
    
    HashTable *ht=create_table(MAX_SIZE);
    parser(filename,ht,vt);
    menu(ht,vt);


    delete_vtable(vt);
    delete_table(ht);
    free(filename);
    return 0;
}

