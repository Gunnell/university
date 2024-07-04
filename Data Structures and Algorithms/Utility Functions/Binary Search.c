#include<stdio.h>
int main(){
    
    int dizi[]={1,3,5,6,8,12,22};
    int N=6;
    int i=0;
    int first=i;
    int last=N;
    int mid=(first+last)/2;
    int find=7;
    
    while(first<=last && dizi[mid]!=find){
        if(find>dizi[mid])
            first=mid+1;
                else
            last=mid-1;
        mid=(first+last)/2;
    }
    if(first>last)
        printf("The number %d have not been found on the array\n",find);
      
    else
        printf("The number %d have been found on the %d index of the array\n",find,mid);

    return 0;
    
}
