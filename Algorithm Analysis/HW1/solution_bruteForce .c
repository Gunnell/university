#include <stdio.h>
#include <stdlib.h>

int getMaxSubarray(int [], int n, int *idx_i, int *idx_j);

int main(void){
    int i; // index of the for loop used while entering the array manually
    int n ; // variable to hold the size of the array
    int maxProfit; // variable to hold the maximum profit
    int array[] = { -2, -1, -3, -4, -1, -2, 1, 0, 2, -1 };
    n = sizeof(array) / sizeof(array[0]); // size when an array is given
    int iSubIdx, jSubIdx; // gives the start and end indices of the block that provides the best profit
    
    /*
       printf("Please enter the size of the array: \n");
       scanf("%d", &n);
       while(n<=0){
           printf("Size of array must be a nonzero positive integer. Please try again: \n");
           scanf("%d", &n);
            }
       int* array = (int*)malloc(n * sizeof(int));
       if (array == NULL) {
               printf("Memory not allocated.\n");
               return -1;
           }
           for(i=0;i<n;i++){
               printf("Enter %d. element: \n", i+1);
               scanf("%d", &array[i]);
                     } */
    
    maxProfit = getMaxSubarray(array, n, &iSubIdx, &jSubIdx);

    if(maxProfit == 0){
        printf("The miner will not get any profit. :( Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, iSubIdx, jSubIdx);
        
    }
    else if(maxProfit < 0){
        printf("The miner will not get any profit but a waste of effort.:( Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, iSubIdx, jSubIdx);
        
    }
    else {
        printf("Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, iSubIdx, jSubIdx);
        
    }
           
    return 0;
}

int getMaxSubarray(int arr[], int n, int *iSubIdx, int *jSubIdx){
    
    int i, j; // indices used to traverse the array
    int max = arr[0];
    int sum;
    *iSubIdx = 0; // pointers used to assign the start and end indices of the last found block to the main address
    *jSubIdx = 0;
    
    for (i=0; i<n; i++){
        sum = 0; // sum is reset for the next iteration
        for (j=i; j<n; j++){
            sum += arr[j];
                if (sum > max) { 
                    max = sum;
                    *iSubIdx = i;
                    *jSubIdx = j;
                }
        }
    }

    return max;
}
