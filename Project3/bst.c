#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

BstNode * BSTnewNode(Date *d1,int flag){
    BstNode* BstNew= (BstNode*)malloc(sizeof(BstNode));
    BstNew->date = malloc(sizeof(Date));
    BstNew->date->year = d1->year;
    BstNew->date->month = d1->month;
    BstNew->date->day = d1->day;
    BstNew->left = BstNew->right = NULL;
    BstNew->accepted_requests=0;
    BstNew->failed_requests=0;
    if(flag==0) BstNew->failed_requests++;
    else BstNew->accepted_requests++;
    return BstNew;
}

BstNode* BSTinsert(BstNode* node, Date *d1,int flag){
    if (node == NULL)
        return BSTnewNode(d1,flag);

    if (Compare_Dates(d1,node->date)==-1)
        node->left = BSTinsert(node->left, d1,flag);
    else if (Compare_Dates(d1,node->date)==1)
        node->right = BSTinsert(node->right, d1,flag);
    else{
        if(flag==0){
            node->failed_requests++;
        }
        else{
            node->accepted_requests++;
        }
    }

 
    return node;
}

//Function to get the sum of all requests in the tree
void BSTsum(BstNode* node,Date *d1,Date *d2,int *suma,int *sumd){
    if (node == NULL)
        return;
 
    /* We are in the date range count the requests */
    if(Compare_Dates(node->date,d1)>=0 && Compare_Dates(node->date,d2)<=0){
        BSTsum(node->left,d1,d2,suma,sumd);
        (*suma) += node->accepted_requests;
        (*sumd) += node->failed_requests;
        BSTsum(node->right,d1,d2,suma,sumd);
    }
    //The current node's date is less than the first date
    //Go to the right subtree to find the results
    else if(Compare_Dates(node->date,d1)<0){
        BSTsum(node->right,d1,d2,suma,sumd);
    }
    //Likewise for the left subtree
    else if(Compare_Dates(node->date,d2)>0){
        BSTsum(node->left,d1,d2,suma,sumd);
    }
}

//Function to get the sum of all requests in the tree
void BSTsum_simple(BstNode* node,int *suma,int *sumd){
    if (node == NULL)
        return;
    
    BSTsum_simple(node->left,suma,sumd);
    (*suma) += node->accepted_requests;
    (*sumd) += node->failed_requests;
    BSTsum_simple(node->right,suma,sumd);
}

BstNode* BSTminimum(BstNode* node){
    BstNode* cur = node;
 
    while (cur && cur->left != NULL)
        cur = cur->left;
 
    return cur;
}

BstNode* BSTsearch(BstNode* root, Date *d1){
    if (root == NULL || root->date == d1)
       return root;
    
    if (root->date < d1)
       return BSTsearch(root->right, d1);
 
    return BSTsearch(root->left, d1);
}



void BSTdestroy(BstNode* root){
    if(root != NULL){
        BSTdestroy(root->left);
        BSTdestroy(root->right);
        free(root->date);
        free(root);
    }
}
