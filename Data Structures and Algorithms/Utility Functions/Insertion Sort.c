#include <stdio.h>

void sort(int *,int);
int main(){
    int dizi[]={7,6,5,4,3,2,1};
    int N=7,i=0;
    sort(dizi,N);
    
    for(;i<N;i++)
    printf("%d ",dizi[i]);
    printf("\n");
    
    
}
void sort(int *dizi,int N){
    int i,j,tmp;
    
    for(i=1;i<N;i++){
        tmp=dizi[i];
        j=i;
        while(tmp<dizi[j-1] && j>0){
            dizi[j]=dizi[j-1];
            j--;
        }
        dizi[j]=tmp;
        
        
    }
   
}
