#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Record.h"


int Compare_Dates(Date *First, Date * Second){
    if(First->year > Second->year)
        return 1;
    else if(First->year==Second->year && First->month > Second->month)
        return 1;
    else if (First->year==Second->year && First->month == Second->month  &&  First->day >Second->day)     
        return 1 ;
    else if (First->year==Second->year && First->month == Second->month  &&  First->day ==Second->day)     
        return 0 ;
    else return -1;
    
}

int Dates_Difference(Date *First, Date * Second){
    int first = First->year * 365 + First->month*30 + First->day;
    int second = Second->year * 365 + Second->month*30 + Second->day;
    return first - second;
}

int Check_Unique(Record *Fir, Record *Sec){
    if(strcmp(Fir->CitizenId,Sec->CitizenId)==0){
        if(strcmp(Fir->firstname,Sec->firstname)==0 && strcmp(Fir->lastname,Sec->lastname)==0 && Fir->age==Sec->age && strcmp(Fir->Country,Sec->Country)==0)
            return 1;
    }
    return -1;
}



Date *CopyDate(Date *date){
    Date *NewDate=malloc(sizeof(Date));
    NewDate->year=date->year;
    NewDate->month=date->month;
    NewDate->day=date->day;

    return  NewDate;
}



void PrintRecord(Record *record){
    printf("%s %s %s %s %d %s %s",record->CitizenId,record->firstname,record->lastname,record->Country,record->age,record->virusName,record->info);
    if(strcmp(record->info,"YES")==0){
        printf (" %d-%d-%d",record->dateVaccinated->day,record->dateVaccinated->month,record->dateVaccinated->year);
    }
    printf("\n");
}

void print_statistics(int age020,int age2040,int age4060,int age60p,int n){
    printf("0-20 %d %.2f%%  \n",age020,(float)(((float)(100*age020))/(float)n));
    printf("20-40 %d %.2f%%  \n",age2040,(float)(((float)(100*age2040))/(float)n));
    printf("40-60 %d %.2f%%  \n",age4060,(float)(((float)(100*age4060))/(float)n));
    printf("60+ %d %.2f%% \n",age60p,(float)(((float)(100*age60p))/(float)n));
}
