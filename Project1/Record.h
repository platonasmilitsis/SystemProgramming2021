#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>

typedef struct date{
    int day;
    int month;
    int year;
}Date;


typedef struct record{
    char* CitizenId;
    char* firstname;
    char* lastname;
    char* Country;
    int age;
    char* virusName;
    Date* dateVaccinated;
    char* info;
    int num;

}Record;

Date *CopyDate(Date *date);
int Compare_Dates(Date *First, Date * Second);
int Check_Unique(Record *Fir, Record *Sec);
void print_statistics(int age020,int age2040,int age4060,int age60p,int n);
void PrintRecord(Record *record);

#endif //RECORD_H