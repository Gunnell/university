#include<stdio.h>
int fakt(int );
int main(){
    int N=5;
    int sonuc =fakt(N);
        printf("%d\n",sonuc);
    
}
int fakt(int N){
        if(N==1)
        return 1;

       return N*fakt(N-1); //else :D
        
}
