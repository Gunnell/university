#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    
    int value;
     struct Node* next;
    
}NODE;
void insertNode(NODE**,NODE*);
NODE* createNewNode(void);
int main(){
    NODE* head=NULL;
    NODE* newNode;
    newNode=createNewNode();
    insertNode(&head,newNode);
    
    
    
    
}
NODE* createNewNode(void){
    NODE* new_node;
    
    
    new_node=(NODE*)calloc(1,sizeof(NODE));
    printf("Enter the value of new node:\n");
    scanf("%d",&new_node->value);
    new_node->next=NULL;
        return new_node;
}
void insertNode(NODE** headAddr,NODE* newNode){
    
    if(*headAddr==NULL){
        *headAddr=newNode;
        exit(0);
    }
    
}
