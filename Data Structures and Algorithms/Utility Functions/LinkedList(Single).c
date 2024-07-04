#include<stdio.h>
#include<stdlib.h>
struct Node {
    int data;
    struct Node* next;
};
struct Node* createNode();
void reverse(struct Node** head_ref);
void printList(struct Node** head_ref);
int delete(struct Node** head_ref,int value);
void insert(struct Node** head_ref,struct Node* node);

int main(){
    struct Node* head;
    head= NULL;
    struct Node* new_node;
    new_node=(struct Node*)malloc(sizeof(struct Node));
    for(int i=0;i<5;i++){
    new_node=createNode();
    if(new_node==NULL)
        return -1;
    
    insert(&head, new_node);
    }
    printList(&head);
    
    
    
    
    return 0;
}
struct Node* createNode(){
    struct Node* p;
    p=(struct Node*)malloc(sizeof(struct Node));
    if(p==NULL){
        printf("Allocation error");
        return NULL;
    }
    printf("Enter the value\n");
    int value;
    scanf("%d",&value);
    p->data=value;
    p->next=NULL;
    return p;
   
}
void printList(struct Node** head_ref){
    struct Node* tmp=*head_ref;
    
    while(tmp->next!=NULL){
        printf("%d ", tmp->data);
        tmp=tmp->next;
    }
    printf("%d ", tmp->data);
    printf("\n");
    
}

int delete(struct Node** head_ref,int value){
    
    if(head_ref==NULL){
        printf("List is empty\n");
        return 0;
    }
    struct Node* tmp = *head_ref;
    if(tmp->data==value){
        *head_ref=(*head_ref)->next;
        free(tmp);
        
    }
    struct Node* prev = *head_ref;
    struct Node* current = (*head_ref)->next;
    
    while(current!=NULL && current->data!=value){
        prev=current;
        current=current->next;
    }
    if(current==NULL){
        printf("Not found\n");
        return 0;
    }
    prev->next=current->next;
    free(current);
    
    return 1;
}
void insert(struct Node** head_ref,struct Node* new_node){
    
    if((*head_ref) == NULL)
    {
       
        (*head_ref) = new_node;
        return ;
    }
    
    
    struct Node* tmp=*head_ref;
    
        while(tmp->next!= NULL)
        {
           tmp = tmp->next;
        }
         tmp->next = new_node;
       
}
void reverse(struct Node** head_ref)
{
    struct Node* prev = NULL;
    struct Node* current = *head_ref;
    struct Node* next = NULL;
    while (current != NULL) {
        
        next = current->next;
        current->next = prev;
 
        prev = current;
        current = next;
    }
    *head_ref = prev;
}
