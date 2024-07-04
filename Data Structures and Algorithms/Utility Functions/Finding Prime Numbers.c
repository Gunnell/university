#include<stdio.h>
int main(){
    
    int N=97;
    int i=2;
    
    
    while(N%i!=0)
        i++;
    
    if(i==N)
        printf("Prime");
    else
        printf("Not Prime");
    
    printf("\n");
    
}
