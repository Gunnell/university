#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>

#define STR_MAX_LEN 55

// Every word read from the file will be stored in the linked list

// The reason for using a linked list is to avoid reading the file twice. Since I/O operations are more costly, the file will be read once and the information will be stored. 
// The reason for not storing it in the hash table during the first read is that we do not have the N information.

struct node{
    char name[STR_MAX_LEN];
    struct node* next;
};

struct hashNode{
    char web[STR_MAX_LEN];
    char query[STR_MAX_LEN];
    int try; // in which attempt it was placed
};

struct hashTable{
    unsigned size; // hash table size
    struct hashNode** hashNodeArray;
};

unsigned hornerHash(char word[], int M);
int nextSmallestPrime(int m);
bool isPrime(int m);
struct node* readFile(char* filePath, int*);
bool isWebSite(char word[]);
struct hashTable* createHashTable(int M, struct node* head);
struct node* searchHashTable(struct hashTable* hashTable, char* query);
struct node* mergeResults(struct node* r1 , struct node* r2, char* );
void printHashTableDetails(struct hashTable* hashTable);

int main() {

    int j;
    int N; // number of query elements in the file
    int M; // Hash Table size
    float loadFactor; // Load Factor --> alpha value
    struct node* result;
    struct node* result1;
    struct node* result2;
    int outputMode; // flag to hold whether the output result is in normal mode or detailed mode

    char query[STR_MAX_LEN]; // variable to hold the query entered by the user
    char backupQuery[STR_MAX_LEN];
    
    printf("Please enter the query\n");
    scanf ("%[^\n]%*c", query);
    strcpy(backupQuery, query);

    printf("\nPlease enter Load Factor\n");
    scanf("%f", &loadFactor);
    
    // printf("Enter source file path: ");
    // scanf("%s", fileName);
        
    char fileName[] = "Sample.txt"; 
   
    int wordCount = 0; // finds the number of query words in the file
    
    // File reading operations
    struct node* head = readFile(fileName, &wordCount);
    
    N = wordCount; // variable to hold the number of query words while reading the file

    M = nextSmallestPrime(N / loadFactor); // variable to be used to determine array size --> Hash Table Size
    
    // CREATING HASH TABLE
    struct hashTable* hashTable = createHashTable(M, head);
      
    // If the query received from the user contains AND and OR, it is divided.
    char *p = strtok(query, " ");
    char *queryArray[3];
    j = 0;

    while (p != NULL) {
        queryArray[j++] = p;
        p = strtok(NULL, " ");
    }
        
    if (j == 3) {
        result1 = searchHashTable(hashTable, queryArray[0]); // If the query also contains AND and OR words, search in the hash table accordingly and merge the results.
        result2 = searchHashTable(hashTable, queryArray[2]);
        result = mergeResults(result1, result2, queryArray[1]);
    } else {
        result = searchHashTable(hashTable, query);
    }
            
    printf("\n\nFor 'Normal Output Mode' enter 0\nFor 'Detailed Output Mode' enter 1\n");
    scanf("%d", &outputMode);
    
    if (outputMode) { // detailed mode
        printHashTableDetails(hashTable);
    }
    
    if (result == NULL) {
        printf("\n\nThere is no website matching query: %s\n", backupQuery);
    } else {
        printf("\n\nResult of query: %s \n\n", backupQuery);
        while (result != NULL) {
            printf("%s\n", result->name);
            result = result->next;
        }
    }
    printf("\n");
    return 0;
}

struct node* readFile(char fileName[], int* wordCount) {
    struct node* head = NULL;
    FILE *inFile = fopen(fileName, "r");
    char readWord[100]; 
    if (inFile == NULL) {
        printf("Error! Unable to read the file\n");
        return NULL;
    }
    while (!feof(inFile)) {
        fscanf(inFile, "%s ", readWord);
        if (isWebSite(readWord) != true) {
            (*wordCount)++;
        }
        // add word to the beginning of the linked list
        // control if head is null add the first node
        if (head == NULL) {
            head = malloc(sizeof(struct node));
            strcpy(head->name, readWord);
            head->next = NULL;
        } else {
            struct node* newNode = malloc(sizeof(struct node));
            strcpy(newNode->name, readWord);
            newNode->next = NULL;
            struct node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    return head;
}

bool isWebSite(char word[]) {
    if (strstr(word, "https://") == word) {
        return true;
    } else {
        return false;
    }
}

bool isPrime(int m) { // finds if a number is prime
    int i;
    int sq_root = (int)sqrt(m);
    for (i = 2; i <= sq_root; i++)
        if (m % i == 0)
            return false;
    return true;
}

int nextSmallestPrime(int m) {
    while (!isPrime(m)) {
        m++;
    }
    return m;
}

unsigned hornerHash(char word[], int M) {
    int R = 31;
    long word_len = 0;
    if (word != NULL) {
        word_len = strlen(word);
    }
     
    int i;
    unsigned hash = 0;
    int R_pow = 1;
    
    for (i = 0; i < word_len; i++) {
        hash = hash + (word[i] - 'A' + 1) * R_pow; // The integer equivalent of the string input is found using Horner's method
        R_pow = R_pow * R;
    }

    unsigned hashIdx = hash % M; // The hash index is found by taking the modulus with the hash table size
    
    return hashIdx;
}

struct hashTable* createHashTable(int M, struct node* head) {
    int i;
    unsigned hash_idx;
    struct node* tmp = head; // iterator used to traverse the linked list where the information read from the file is stored
    char webName[STR_MAX_LEN];
    int hashTableSize;
    
    // Creating HashTable
    struct hashTable* hashTable = (struct hashTable*)malloc(sizeof(struct hashTable));
    hashTable->size = M;
    hashTable->hashNodeArray = (struct hashNode**)malloc(M * sizeof(struct hashNode*));
    for (i = 0; i < M; i++) {
        hashTable->hashNodeArray[i] = NULL;
    }
    
    // Filling the HashTable
    hashTableSize = hashTable->size;
    
    while (tmp != NULL) {
        i = 0;
        if (isWebSite(tmp->name)) { // if the information we read is website information, we assign it to the webName variable
            strcpy(webName, tmp->name);
        } else {
            hash_idx = hornerHash(tmp->name, M); // if the information we read is query information, we get the corresponding index using Horner's method

            // Assigning the webName information to the obtained index
            while (i < hashTableSize && hashTable->hashNodeArray[hash_idx] != NULL) {
                hash_idx = (hash_idx + 1) % M; // in case of collision, the index is obtained with linear probing
                i++;
            }
            if (i == hashTableSize) {
                printf("Hash table is full\n");
            } else {
                hashTable->hashNodeArray[hash_idx] = (struct hashNode*)malloc(sizeof(struct hashNode));
                strcpy(hashTable->hashNodeArray[hash_idx]->web, webName);
                strcpy(hashTable->hashNodeArray[hash_idx]->query, tmp->name);
                hashTable->hashNodeArray[hash_idx]->try = i;
            }
        }
        tmp = tmp->next;
    }
    
    return hashTable;
}

struct node* searchHashTable(struct hashTable* hashTable, char* query) {
    struct node* head = NULL;
    struct node* tmp = NULL;
    int hashTableSize = hashTable->size;
    int hashIdx = hornerHash(query, hashTableSize);
    int i = 0;
    
    while (i < hashTableSize && hashTable->hashNodeArray[hashIdx] != NULL) {
        if (strcmp(hashTable->hashNodeArray[hashIdx]->query, query) == 0) {
            if (head == NULL) {
                head = (struct node*)malloc(sizeof(struct node));
                strcpy(head->name, hashTable->hashNodeArray[hashIdx]->web);
                head->next = NULL;
                tmp = head;
            } else {
                tmp->next = (struct node*)malloc(sizeof(struct node));
                strcpy(tmp->next->name, hashTable->hashNodeArray[hashIdx]->web);
                tmp->next->next = NULL;
                tmp = tmp->next;
            }
        }
        hashIdx = (hashIdx + 1) % hashTableSize; // linear probing is used during search as it was used during insertion.
        i++;
    }
    return head;
}

struct node* mergeResults(struct node* r1 , struct node* r2, char conjWord[]) { // merges the results if the input contains the words "and" or "or"

    struct node* head = NULL;

    if (strcmp(conjWord, "and") == 0) {
        while (r1 != NULL) {
            struct node* tmp = r2;
            while (tmp != NULL && strcmp(tmp->name, r1->name) != 0) {
                tmp = tmp->next;
            }
            if (tmp != NULL) {
                struct node* newNode = (struct node*)malloc(sizeof(struct node));
                strcpy(newNode->name, r1->name);
                newNode->next = head;
                head = newNode;
            }
            r1 = r1->next;
        }
        return head;
    } else if (strcmp(conjWord, "or") == 0) {
        head = r2;
        while (r1 != NULL) {
            struct node* tmp = r2;
            
            while (tmp != NULL && strcmp(tmp->name, r1->name) != 0) {
                tmp = tmp->next;
            }
            
            if (tmp == NULL) {
                struct node* newNode = (struct node*)malloc(sizeof(struct node));
                strcpy(newNode->name, r1->name);
                newNode->next = head;
                head = newNode;
            }
            r1 = r1->next;
        }
        return head;
    }

    return NULL;
}

void printHashTableDetails(struct hashTable* hashTable) {
    // Function that prints the desired outputs in detailed mode
    int M = hashTable->size;
    int i;
    
    printf("\n\n-------------HASH TABLE DETAILS-------------\n\n");
    printf("Size of Hash Table: %d\n\n", M);
    printf("Content of Hash Table: \n\n");
    
    for (i = 0; i < M; i++) { // items 2 and 3 are combined and displayed
        if (hashTable->hashNodeArray[i] != NULL) {
            printf("[%d] %s--> %s || try: %d\n", i,
                   hashTable->hashNodeArray[i]->query,
                   hashTable->hashNodeArray[i]->web,
                   hashTable->hashNodeArray[i]->try);
        }
    }
}
