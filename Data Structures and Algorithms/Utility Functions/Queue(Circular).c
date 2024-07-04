#include <stdio.h>
#include <stdlib.h>
#define SIZE 50
struct Queue{
    int front;
    int rear;
    int items[SIZE];
    };
void initQueue(struct Queue* );
int isEmpty(struct Queue* );
int isFull(struct Queue* kuyruk);
int enQueue(struct Queue* kuyruk,int element);
int deQueue(struct Queue* kuyruk);
void printQueue(struct Queue* );



int main() {
    
    struct Queue* kuyruk;
    kuyruk=(struct Queue*)malloc(sizeof(struct Queue));
    
    if(kuyruk==NULL){
        printf("Allocation error\n");
        return -1;
    }
    initQueue(kuyruk);
    

    return 0;
}
void initQueue(struct Queue* kuyruk){
    
    kuyruk->front=-1;
    kuyruk->rear=-1;
}

int isEmpty(struct Queue* kuyruk){
    if(kuyruk->front==-1)
        return 1; //bos
    return 0; //dolu

    
}
int isFull(struct Queue* kuyruk){
    if( (kuyruk->front==(kuyruk->rear)+1) || ((kuyruk->front==0) && (kuyruk->rear==SIZE-1)) ){
        
        return 1; // dolu
        
    }
       
    return 0; //bos

    
}
int enQueue(struct Queue* kuyruk,int element){
    if(isFull(kuyruk))
        return 0;
    else if(kuyruk->front==-1){
        kuyruk->front=0;
        kuyruk->rear=((kuyruk->rear)+1)%SIZE;
        kuyruk->items[kuyruk->rear]= element;
       
    }
    return 1;
}
int deQueue(struct Queue* kuyruk){
    if(isEmpty(kuyruk))
        return 0;
    else if(kuyruk->front==kuyruk->rear){
        kuyruk->front=-1;
        kuyruk->rear=-1;
       
    }
    else{
        kuyruk->front=((kuyruk->front)+1)%SIZE;
        
    }
    return 1;
}
