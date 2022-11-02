#ifndef BST_H
#define BST_H

#include "Record.h"

typedef struct bst_node
{
    Date *date;
    int accepted_requests;
    int failed_requests; 
    struct bst_node *right;  
    struct bst_node *left; 
}BstNode;

BstNode * BSTnewNode(Date *d1,int flag);
BstNode* BSTminimum(BstNode* node);
void BSTsum(BstNode* node,Date *d1,Date *d2,int *suma,int *sumd);
BstNode* BSTsearch(BstNode* root, Date *d1);
BstNode* BSTinsert(BstNode* node, Date *d1,int flag);
void BSTsum_simple(BstNode* node,int *suma,int *sumd);
void BSTdestroy(BstNode* root);

#endif //BST_H