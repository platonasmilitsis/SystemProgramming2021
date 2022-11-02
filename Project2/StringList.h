#ifndef STRINGLIST_H
#define STRINGLIST_H

typedef struct StringNode {
    char *value;
    struct StringNode *next;
}StringNode;

typedef struct countryList{
    int size;
    StringNode *head;
    StringNode *tail;
}StringList;


StringList *StringList_create();
StringList *StringList_Insert(StringList *CList, char* value);
StringNode *Create_string_node(char* value);
int StringList_Search(StringList *SList, char* value);
char *StringList_to_string(StringList *SList);
void StringList_Destroy(StringList *SList);
void StringList_print(StringList *SList);

#endif //STRINGLIST_H