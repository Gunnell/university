#include <stdio.h>
#include <stdlib.h>

int getMaxSubarray(int arr[], int l, int h, int*, int*);
int getCrossingSubarray(int arr[], int l, int h, int mid, int*, int*);

int main(){
    int arr[] = { -2, -1, -3, -4, -1, -2, 1, 0, 2, -1};
    int n = sizeof(arr) / sizeof(arr[0]);
    int idxArr[] = {0,0};
    int start = 0, stop = n - 1; // start and end indices of the final region
    int maxProfit = 0;
    
    /* // Manually taking the array
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
    
    maxProfit = getMaxSubarray(arr, 0, n - 1, &start, &stop);
    
    if(maxProfit == 0){
        printf("The miner will not get any profit. :( Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, start, stop);
        
    }
    else if(maxProfit < 0){
        printf("The miner will not get any profit but a waste of effort. :( Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, start, stop);
        
    }
    else {
        printf("Maximum profit is: %d\nblocks: %d-%d.\n\n", maxProfit, start, stop);
        
    }
    
    return 0;
}

int getMaxSubarray(int arr[], int l, int h, int* start, int* stop) { 
    // This function returns the final max value. Recursively returns the maximum of the left part, right part, 
    // and the sum of the values in the middle of these two parts
    
    if (l > h) { // l is the lowest index of the array, h is the highest index
        printf("Invalid values for low and high indexes. Low index must be equal or less than high index");
        return -1;
    }
    
    if (l == h) { // case of a single element in the array
        *start = l;
        *stop = l;
        return arr[l];
    }
     
    int mid = (l + h) / 2; // index of the middle element of the array
    int max;
    int leftSubarrayMax; // max value of the left part
    int rightSubarrayMax; // max value of the right part
    int crossingSubarrayMax; // max part that remains in the middle of the left and right
    int startLeft, stopLeft; // max value indices of the left part
    int startRight, stopRight; // max value indices of the right part
    int startCross, stopCross; // max value indices of the middle part
    
    leftSubarrayMax = getMaxSubarray(arr, l, mid, start, stop);
    startLeft = *start;
    stopLeft = *stop;
    rightSubarrayMax = getMaxSubarray(arr, mid + 1, h, start, stop);
    startRight = *start;
    stopRight = *stop;
    crossingSubarrayMax = getCrossingSubarray(arr, l, h, mid, start, stop);
    startCross = *start;
    stopCross = *stop;
    
    if (leftSubarrayMax > rightSubarrayMax) {
        max = leftSubarrayMax;
        *start = startLeft;
        *stop = stopLeft;
    } else {
        *start = startRight;
        *stop = stopRight;
        max = rightSubarrayMax;
    }
    
    if (crossingSubarrayMax > max) {
        *start = startCross;
        *stop = stopCross;
        max = crossingSubarrayMax;
    }
    
    return max;
}

int getCrossingSubarray(int arr[], int l, int h, int mid, int* start, int* stop) {
    int i;
    int i_r, j_r; // holds the start and end indices of the right max
    int i_l, j_l; // holds the start and end indices of the left max
    int startIdx, stopIdx; // holds the start and end indices of the final max region
    int sum;
    int leftMax = arr[mid]; // holds the max of the region remaining on the right side
    int rightMax = arr[mid + 1]; // holds the max of the region remaining on the left side
    int crossSum; // holds the sum of the regions between the right and left regions
    int max; // holds the final max result
    
    sum = 0; // finds the max subarray of the left region
    j_l = mid;
    i_l = mid;
    for (i = mid; i >= l; i--) { // moves from the middle to the left for the left region
        sum += arr[i];
        if (sum > leftMax) {
            leftMax = sum;
            i_l = i;
        }
    }
    
    sum = 0; // finds the max subarray of the right region
    i_r = mid + 1;
    j_r = mid + 1;
    for (i = mid + 1; i <= h; i++) { // moves from the middle to the right for the right region.
        sum += arr[i];
        if (sum > rightMax) {
            rightMax = sum;
            j_r = i;
        }
    }
    
    crossSum = leftMax + rightMax; // holds the value of the region between the right and left regions
    
    if (leftMax > rightMax) {
        max = leftMax;
        startIdx = i_l;
        stopIdx = j_l;
    } else {
        max = rightMax;
        startIdx = i_r;
        stopIdx = j_r;
    }
    
    if (crossSum > max) {
        max = crossSum;
        startIdx = i_l;
        stopIdx = j_r;
    }
    
    *start = startIdx; // holds the indices of the returned max value
    *stop = stopIdx;
    
    return max;
}
