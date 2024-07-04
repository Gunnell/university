#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//From 1 to N prime numbers
int main(){
    int N=36;
    int *dizi=(int *)calloc(N,sizeof(int));
    int bound=sqrt(N);
    int  i=4,j;
    dizi[1]=1;
    for(;i<=N;i=i+2)
        dizi[i]=1; //even numbers are not prime implicitly
   
    
    for(i=3;i<=bound;i=i+2){
        if(dizi[i]==0)
                for(j=i*i;j<=N;j+=i*2){
            dizi[j]=1;
        }

        
    }
    
    for(i=1;i<=N;i++)
        printf("%d(%d)  ",dizi[i],i);
    printf("\n");
    
    
    
}
