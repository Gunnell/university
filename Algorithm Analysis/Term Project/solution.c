#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<string.h>

#define COLOR_LEN 20

typedef struct color{
    int colorCode;
    char colorName[COLOR_LEN];
}COLOR;

int insert(COLOR *colors, char *color, int N);
bool checkMatrix(int** matrix, int row, int N);
void shiftRight(int** matrix, int row, int N);
void printMatrix(int** matrix, int N, COLOR* colors);
int playGame(int** matrix, int row, int N, int mode, COLOR* colors);

int main(){
    int N; // size of the game board
    int i, j; // variables used in loops
    int** board; // game board
    char color[COLOR_LEN]; // colors taken from the user
    int mode; // detailed or normal mode variable

    printf("Please enter the size of the board:\n");
    scanf("%d", &N);
    
    while(N<=2 || N>=9){
        printf("Please re-enter the size.\nMake sure size is 3 <= size <= 8.\n");
        scanf("%d", &N);
    }
    
    // initializing the game board
    board = malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        board[i] = malloc(N * sizeof(int));
    }
    
    // initializing the color array
    COLOR *colors = malloc(N * sizeof(COLOR));
    for (i = 0; i < N; i++) {
        colors[i].colorCode = -1;
        strcpy(colors[i].colorName, "");
    }
    
    // taking the initial state of the game board from the user
    printf("Please enter the initial state of the board:\n");
    for(i = 0; i < N; i++){
        printf("Enter the colors at the %.d :\n", i + 1);
        for(j = 0; j < N; j++){
            scanf("%s", color);
            board[i][j] = insert(colors, color, N);
        }
    }
    
    printf("\n----------------------------\n");
    printf("Please enter the 0 for the standard mode\nand 1 for the detailed mode: ");
    scanf("%d", &mode);
    
    if (playGame(board, 0, N, mode, colors)){
        printf("\n-----Success-----\n");
        if(mode == 0){
            printf("\nThe last state \n");
            printf("\n-----------------\n");
            printMatrix(board, N, colors);
            printf("\n-----------------\n");
        }
    }
    else {
        printf("\n-----Failure-----\n");
        printf("\n-----------------\n");
        printMatrix(board, N, colors);
        printf("-----------------\n");
    }
    
    printf("\n");

    return 0;
}

int insert(COLOR *colors, char *color, int N){
    // creates the color array.
    
    int i = 0;
    while(i < N && strcmp(colors[i].colorName, color) != 0 && colors[i].colorCode != -1)
        i++;
    if(i == N)
        return -1;
    if (colors[i].colorCode == -1){
        colors[i].colorCode = i + 1;
        strcpy(colors[i].colorName, color);
        return i + 1;
    }
    return colors[i].colorCode;
}

bool checkMatrix(int** matrix, int row, int N){
    // checks each element in each column while traversing the matrix. Returns immediately upon first match
    int i, j, k;
    for (j = 0; j < N; j++) {
        for (i = row; i > 0; i--) {
            for (k = i - 1; k >= 0; k--) {
                if (matrix[i][j] == matrix[k][j]) {
                    return false; // found duplicate element
                }
            }
        }
    }
    return true;
}

void shiftRight(int** matrix, int row, int N){
    // shifts the given row once to the right
    int j;
    int x = matrix[row][N - 1];
    for (j = N - 1; j > 0; j--)
        matrix[row][j] = matrix[row][j - 1];
    matrix[row][0] = x;
}

void printMatrix(int** matrix, int N, COLOR* colors){
    // prints the matrix
    int i, j;
    int colorCode;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            colorCode = matrix[i][j] - 1;
            printf("%s\t", colors[colorCode].colorName);
        }
        printf("\n");
    }
}

int playGame(int** matrix, int row, int N, int mode, COLOR* colors){
    // a function that progresses recursively with DFS logic
    int i;
    if (row == N){ // base (stop) case of the recursive function. The entire matrix has been traversed. This case is not reached if there is no result in the case where there is no result. Control case to prevent infinite loop.
        return 1;
    }
    
    for (i = 0; i < N; i++){
        if (checkMatrix(matrix, row, N)){ // each time the function is called, the similarity of the elements in the column is checked first
            if (playGame(matrix, row + 1, N, mode, colors)){
                return 1;
            }
        }
        shiftRight(matrix, row, N); // shifts right in case of similarity
        if(mode){
            printf("Shift Result\n"); // prints the state space tree.
            printMatrix(matrix, N, colors);
        }
    }
    
    return 0;
}
