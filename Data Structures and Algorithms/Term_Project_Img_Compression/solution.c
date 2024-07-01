#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_VAL 0
#define MAX_VAL 255
#define ERROR_MALLOC "Malloc error has occurred\n"
#define ERROR_FILE  "File cannot be opened\n"

typedef struct imgData {
    int row;
    int col;
    int maxColor;
    int **matrix;
} PGMimage;

typedef struct node {
    int freq;
    int code;
    struct node* next;
} NODE;

void readImage(char*, PGMimage*);
void RLE(PGMimage*, NODE**);
void createAndInsertNewNode(NODE** head, int counter, int color);
void writeToFile(NODE** head, int col, int row, int maxColor);
void freeMatrix(int** img, int row);
PGMimage* unRLE(char*);
bool control(char*);
void writeDecodedToFile(PGMimage*);
int changeColor(NODE** head, int max);
void changeColorByCoordinate(NODE** head, int x, int y, int newColor, int col);
void calculateHistogram(NODE**);
void histList(NODE** head, NODE* current);
void histogram(NODE*);

int main() {
    char* fileName = "lena.pgm";
    char* compressedFileName = "text_encoded.txt";
    NODE* head = NULL;
    int opt = 0;
    int max;
    PGMimage* img = malloc(sizeof(PGMimage));
    if (img == NULL) {
        printf(ERROR_MALLOC);
        printf("\n");
        return -1;
    }

    readImage(fileName, img);
    RLE(img, &head);
    max = img->maxColor;

    printf("Compressed Text:\n\n");
    NODE* tmp = head;
    while (tmp != NULL) {
        printf("%d: %d\n", tmp->freq, tmp->code);
        tmp = tmp->next;
    }

    PGMimage* img2 = unRLE(compressedFileName);
    if (img2 == NULL) {
        printf("An error occurred while opening and processing 'text_encoded.txt', so image cannot be represented as .pgm\n");
    } else {
        writeDecodedToFile(img2);
    }

    printf("\n\n**************************************\n\n");
    while (opt != -1) {
        printf("Please select operation you want to proceed\n"
               " 1: Change the color of the pixels\n"
               " 2: Change the color of the certain pixel by coordinates\n"
               " 3: Calculate the histogram\n"
               " 4: Save changes to File and See the processed image\n"
               "-1: Exit\n\n");
        scanf("%d", &opt);
        switch (opt) {
            case 1:
                max = changeColor(&head, img->maxColor);
                break;
            case 2: {
                int x, y, newColor;
                printf("Enter the coordinates (x y) and new color: ");
                scanf("%d %d %d", &x, &y, &newColor);
                changeColorByCoordinate(&head, x, y, newColor, img->col);
                break;
            }
            case 3:
                printf("<--------------------------->\n    Histogram of an Image        \n\n");
                histogram(head);
                break;
            case 4:
                writeToFile(&head, img->col, img->row, max);
                PGMimage* img3 = unRLE(compressedFileName);
                writeDecodedToFile(img3);  // modified image saved as "text_decoded.pgm"
                freeMatrix(img3->matrix, img3->row);
                free(img3);
                break;
            case -1:
                break;
            default:
                printf("Please select the valid operation\n\n");
                break;
        }
    }

    freeMatrix(img->matrix, img->row);
    free(img);
    if (img2 != NULL) {
        freeMatrix(img2->matrix, img2->row);
        free(img2);
    }
    printf("**************************************\n");

    return 0;
}

void readImage(char fileName[], PGMimage* img) {
    FILE* file;
    char chr[4];
    int i, j;
    int** matrix;

    file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Unable to open file %s\n", fileName);
        exit(1);
    }

    chr[0] = getc(file);
    chr[1] = getc(file);

    if (strcmp(chr, "P2") != 0) {
        printf("Magic number of file is not valid number for P2\n");
        exit(2);
    }
    while (getc(file) != '\n');

    while (getc(file) == '#') {
        while (getc(file) != '\n');
    }
    fseek(file, -1, SEEK_CUR);

    fscanf(file, "%d", &(img->col));
    fscanf(file, "%d", &(img->row));
    fscanf(file, "%d", &(img->maxColor));

    matrix = (int**)malloc(img->row * sizeof(int*));
    if (matrix == NULL) {
        printf(ERROR_MALLOC);
        printf("\n");
        exit(3);
    }

    for (i = 0; i < img->row; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * img->col);
        if (matrix[i] == NULL) {
            printf(ERROR_MALLOC);
            printf("\n");
            exit(4);
        }
    }
    img->matrix = matrix;

    for (i = 0; i < img->row; i++) {
        for (j = 0; j < img->col; j++) {
            fscanf(file, "%d", &(img->matrix[i][j]));
        }
    }

    fclose(file);
}

void RLE(PGMimage* img, NODE** head) {
    int i, j, counter = 0, elem1, elem2;
    int N = img->row, M = img->col;

    elem1 = img->matrix[0][0];
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            elem2 = img->matrix[i][j];
            if (elem1 == elem2) {
                counter++;
            } else {
                createAndInsertNewNode(head, counter, elem1);
                elem1 = elem2;
                counter = 1;
            }
        }
    }
    createAndInsertNewNode(head, counter, elem1);
    writeToFile(head, img->col, img->row, img->maxColor);
}

void createAndInsertNewNode(NODE** head, int counter, int color) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (newNode == NULL) {
        printf(ERROR_MALLOC);
        printf("\n");
        exit(5);
    }

    newNode->freq = counter;
    newNode->code = color;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    NODE* tmp = *head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

void writeToFile(NODE** head, int col, int row, int maxColor) {
    FILE *pgmFile = fopen("text_encoded.txt", "wb");
    if (pgmFile == NULL) {
        printf(ERROR_FILE);
        printf("\n");
        exit(8);
    }

    fprintf(pgmFile, "%d %d %d\n", col, row, maxColor);

    NODE* varHead = *head;
    while (varHead != NULL) {
        fprintf(pgmFile, "%d %d ", varHead->freq, varHead->code);
        varHead = varHead->next;
    }

    fclose(pgmFile);
}

void freeMatrix(int** img, int row) {
    for (int i = 0; i < row; ++i) {
        free(img[i]);
    }
    free(img);
}

bool control(char* fileName) {
    int col, row, maxColor, sum = 0;
    int x0 = -1, x1;
    FILE* file = fopen(fileName, "rb");

    if (file == NULL) {
        printf(ERROR_FILE);
        exit(9);
    }
    fscanf(file, "%d %d %d", &col, &row, &maxColor);

    while (fgetc(file) != EOF) {
        fscanf(file, "%d", &x1);
        sum += x1;
        fscanf(file, "%d", &x1);

        if (x1 > maxColor || x1 < 0) {
            printf("Color code is either greater than image's max value or a negative value\n\n");
            return false;
        }

        if (x0 == x1) {
            printf("File is not compressed correctly, consecutive pixels have not been added\n\n");
            return false;
        } else {
            x0 = x1;
        }
    }

    if (sum != (col * row)) {
        printf("File is not compressed correctly, incorrect pixel size\n\n");
        return false;
    }

    fclose(file);
    return true;
}

PGMimage* unRLE(char* fileName) {
    PGMimage* img = malloc(sizeof(PGMimage));
    FILE* file;
    int i, j = 0, z, k;
    int** matrix;

    if (!control(fileName)) {
        printf("File is not appropriate for decoding\n");
        return NULL;
    }

    file = fopen(fileName, "rb");
    if (file == NULL) {
        printf(ERROR_FILE);
        printf("\n");
        return NULL;
    }

    fscanf(file, "%d %d %d", &img->col, &img->row, &img->maxColor);
    matrix = (int**)malloc(img->row * sizeof(int*));
    if (matrix == NULL) {
        printf(ERROR_MALLOC);
        printf("\n");
        return NULL;
    }

    for (i = 0; i < img->row; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * img->col);
        if (matrix[i] == NULL) {
            printf(ERROR_MALLOC);
            printf("\n");
            return NULL;
        }
    }
    img->matrix = matrix;

    fscanf(file, "%d %d", &z, &k);
    i = 0;
    while (!feof(file) && i < img->row) {
        while (z > 0 && j < img->col) {
            img->matrix[i][j] = k;
            z--;
            j++;
        }
        if (j == img->col) {
            j = 0;
            i++;
        }
        if (z == 0) {
            fscanf(file, "%d %d", &z, &k);
        }
    }

    fclose(file);
    return img;
}

void writeDecodedToFile(PGMimage* img) {
    char* fileName = "text_decoded.pgm";
    FILE* file = fopen(fileName, "wb");
    if (file == NULL) {
        printf(ERROR_FILE);
        return;
    }

    fprintf(file, "%s\n", "P2");
    fprintf(file, "%d %d\n", img->col, img->row);
    fprintf(file, "%d\n", img->maxColor);

    for (int i = 0; i < img->row; i++) {
        for (int j = 0; j < img->col; j++) {
            fprintf(file, "%d ", img->matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int changeColor(NODE** head, int max) {
    int color, change;
    NODE* current = *head;
    printf("Please enter the color you want to change\n");
    scanf("%d", &color);
    printf("Please enter the new color \n");
    scanf("%d", &change);

    while (change > MAX_VAL || change < MIN_VAL) {
        printf("Please enter in the 0-255 range\n");
        scanf("%d", &change);
    }
    if (change > max)
        max = change;

    while (current != NULL) {
        if (current->code == color) {
            current->code = change;
        }
        current = current->next;
    }

    current = *head;
    while (current != NULL && current->next != NULL) {
        if (current->code == current->next->code) {
            current->freq = current->freq + current->next->freq;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }

    return max;
}

void changeColorByCoordinate(NODE** head, int x, int y, int newColor, int col) {
    int position = x * col + y;
    NODE* current = *head;
    int sum = 0;

    while (current != NULL) {
        sum += current->freq;
        if (sum > position) {
            current->code = newColor;
            break;
        }
        current = current->next;
    }

    // Recheck for consecutive color runs
    current = *head;
    while (current != NULL && current->next != NULL) {
        if (current->code == current->next->code) {
            current->freq += current->next->freq;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void calculateHistogram(NODE** head) {
    NODE* current = *head;
    NODE* histHead = NULL;

    while (current != NULL) {
        histList(&histHead, current);
        current = current->next;
    }

    while (histHead != NULL) {
        printf("%d: %d\n", histHead->freq, histHead->code);
        histHead = histHead->next;
    }
}

void histList(NODE** head, NODE* current) {
    NODE* tmp = *head;
    NODE* newNode = (NODE*)malloc(sizeof(NODE));

    newNode->code = current->code;
    newNode->freq = current->freq;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    while (tmp != NULL) {
        if (tmp->code == current->code) {
            tmp->freq = tmp->freq + current->freq;
            return;
        }
        tmp = tmp->next;
    }

    tmp = newNode;
}

void histogram(NODE* node) {
    int i;
    int counted[256];
    for (i = 0; i < 256; i++) {
        counted[i] = -1;
    }
    while (node != NULL) {
        if (counted[node->code] != -1) {
            counted[node->code] += node->freq;
        } else {
            counted[node->code] = node->freq;
        }
        node = node->next;
    }
    for (i = 0; i < 256; i++) {
        if (counted[i] != -1) {
            printf("%d: %d\n", counted[i], i);
        }
    }
}
