#include <stdio.h>
#define MAX 10
typedef struct{
    int item[MAX];
    int Sptr;
}STACK;

void init(STACK* s);
int isEmpty(STACK* );
int isFull(STACK*);
int push(STACK*,int);
int pop(STACK*,int*);
int peek(STACK*,int*);

void init(STACK* s){
    s->Sptr=0;
    }

int isEmpty(STACK* s){
    if(s->Sptr==0)
        return 1;
    return 0 ;
}
int isFull(STACK* s){
    if(s->Sptr==MAX)
        return 1 ;
    return 0;
}

int push(STACK* s,int x ){
    if(isEmpty(s)){
        s->item[s->Sptr]=x;
        ++s->Sptr;
        return 1 ;
    }
    
    return 0;
}

int pop(STACK* s,int* x){
    if(isEmpty(s))
        return 0;
    --s->Sptr;
    *x=s->item[s->Sptr];
   
    return 1;
}
int peek(STACK* s,int* x){
    if(isEmpty(s))
        return 0;

    int adr=--s->Sptr;
    *x=s->item[adr];
    
    return 1;
}
int main() {

}
