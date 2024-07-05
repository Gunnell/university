#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define STR_MAX_LEN 55

struct node {
    char fileString[STR_MAX_LEN];
    struct node* next;
};

struct ADJ_NODE {
    int vertexNo;
    struct ADJ_NODE* next;
};

struct HEAD_NODE {
    int vertexId;
    char name[STR_MAX_LEN];
    char surname[STR_MAX_LEN];
    int initialIndegree;
    int inDegree;
    int total;
    bool deletedStatus;
    struct ADJ_NODE* adjNode;
};

struct GRAPH {
    int numOfVertices;
    struct HEAD_NODE** adjList;
};

struct INFLUENCER {
    int vertexId;
    char name[STR_MAX_LEN];
    char surname[STR_MAX_LEN];
    int inDegree;
    int total;
    struct INFLUENCER* next;
};

struct node* readFile(char* filePath, int*);
struct GRAPH* createGraph(int vertexCount, struct node* head);
bool isNameSurname(char* str);
void findInDegree(struct GRAPH* gr);
void deleteVertices(struct GRAPH* gr, int M);
void calculateTotalConnections(struct GRAPH* gr);
void printInfluencers(struct INFLUENCER* head);
struct INFLUENCER* findInfluencers(struct GRAPH* gr, int X, int Y);
int dfs(struct GRAPH* gr, int vertex, bool* visited);

int main() {
    char fileName[] = "socialNET.txt";
    int nodeCount = 0;
    struct node* head = readFile(fileName, &nodeCount);
    struct GRAPH* gr = createGraph(nodeCount, head);
    int M = 1;
    int X = 2;
    int Y = 2;
    findInDegree(gr);
    deleteVertices(gr, M);
    
    int mode;
    printf("0 -> Normal Mode\n1 -> Advanced Mode:\n");
    printf("Please enter the mode:\n");
    scanf("%d", &mode);
    if (mode) {
        printf("                 -----------Detailed Mode-----------\n\n");
        for (int i = 0; i < nodeCount; i++) {
            printf("%d %s %s deleted info:%d initial_indegree:%d current_indegree:%d \n", gr->adjList[i]->vertexId, gr->adjList[i]->name, gr->adjList[i]->surname, gr->adjList[i]->deletedStatus, gr->adjList[i]->initialIndegree,
                   gr->adjList[i]->inDegree);
            struct ADJ_NODE* tempNode = gr->adjList[i]->adjNode;
            printf("outdegree nodes: ");
            while (tempNode != NULL) {
                printf("%d ", tempNode->vertexNo);
                tempNode = tempNode->next;
            }
            printf("\n\n");
        }
    }
    
    calculateTotalConnections(gr);
    findInfluencers(gr, X, Y);
    printf("\n");
    return 0;
}

struct node* readFile(char fileName[], int* nodeCount) {
    struct node* head = NULL;
    FILE *inFile  = fopen(fileName, "r");
    char readWord[100];
    if (inFile == NULL) {
        printf("Error! Unable to read the file\n");
        return NULL;
    }
    int tmpCount = 0;
    while (!feof(inFile)) {
        fscanf(inFile, "%s ", readWord);
        if (head == NULL) {
            head = malloc(sizeof(struct node));
            strcpy(head->fileString, readWord);
            head->next = NULL;
        } else {
            struct node* newNode = malloc(sizeof(struct node));
            strcpy(newNode->fileString, readWord);
            newNode->next = NULL;
            struct node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
            sscanf(newNode->fileString, "%d,", &tmpCount);
            if (tmpCount > *nodeCount) {
                *nodeCount = tmpCount;
            }
        }
    }
    return head;
}

struct GRAPH* createGraph(int vertexCount, struct node* head) {
    int i;
    struct GRAPH* gr = (struct GRAPH*)malloc(sizeof(struct GRAPH));
    gr->numOfVertices = vertexCount;
    gr->adjList = (struct HEAD_NODE**)malloc(vertexCount * sizeof(struct HEAD_NODE*));
    for (i = 0; i < vertexCount; i++) {
        gr->adjList[i] = (struct HEAD_NODE*)malloc(sizeof(struct HEAD_NODE));
        gr->adjList[i]->vertexId = i + 1;
        gr->adjList[i]->inDegree = 0;
        gr->adjList[i]->initialIndegree = 0;
        gr->adjList[i]->total = 0;
        gr->adjList[i]->deletedStatus = false;
        gr->adjList[i]->adjNode = NULL;
    }
    struct node* temp = head;
    int idVal;
    char first[STR_MAX_LEN], last[STR_MAX_LEN];
    int listIdx;
    while (temp != NULL) {
        if (isNameSurname(temp->fileString)) {
            sscanf(temp->fileString, "%d,%[^,],%s", &idVal, first, last);
            listIdx = idVal - 1;
            strcpy(gr->adjList[listIdx]->name, first);
            strcpy(gr->adjList[listIdx]->surname, last);
        } else {
            char* token = strtok(temp->fileString, ",");
            int i = 0;
            int arr[100];
            while (token != NULL) {
                arr[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            int j;
            for (j = 0; j < i; j++) {
                struct ADJ_NODE* newNode = malloc(sizeof(struct ADJ_NODE));
                newNode->vertexNo = arr[j];
                newNode->next = NULL;
                struct ADJ_NODE* tempNode = gr->adjList[listIdx]->adjNode;
                if (tempNode == NULL) {
                    gr->adjList[listIdx]->adjNode = newNode;
                } else {
                    newNode->next = tempNode;
                    gr->adjList[listIdx]->adjNode = newNode;
                }
            }
        }
        temp = temp->next;
    }
    return gr;
}

bool isNameSurname(char* str) {
    char *p = str;
    while (*p != '\0') {
        if (isalpha(*p) && isupper(*p)) {
            return true;
        }
        p++;
    }
    return false;
}

void findInDegree(struct GRAPH* gr) {
    int i;
    for (i = 0; i < gr->numOfVertices; i++) {
        struct ADJ_NODE* tempNode = gr->adjList[i]->adjNode;
        while (tempNode != NULL) {
            gr->adjList[tempNode->vertexNo - 1]->inDegree++;
            gr->adjList[tempNode->vertexNo - 1]->initialIndegree++;
            tempNode = tempNode->next;
        }
    }
}

void deleteVertices(struct GRAPH* gr, int M) {
    int i;
    for (i = 0; i < gr->numOfVertices; i++) {
        if (gr->adjList[i]->inDegree < M && gr->adjList[i]->deletedStatus == false) {
            gr->adjList[i]->deletedStatus = true;
            struct ADJ_NODE* tempNode = gr->adjList[i]->adjNode;
            while (tempNode != NULL) {
                gr->adjList[tempNode->vertexNo - 1]->inDegree--;
                tempNode = tempNode->next;
            }
            deleteVertices(gr, M);
        }
    }
}

void calculateTotalConnections(struct GRAPH* gr) {
    bool* visited = (bool*)malloc(gr->numOfVertices * sizeof(bool));
    for (int i = 0; i < gr->numOfVertices; i++) {
        memset(visited, false, gr->numOfVertices * sizeof(bool));
        gr->adjList[i]->total = dfs(gr, i, visited) - 1; // Exclude self
    }
    free(visited);
}

int dfs(struct GRAPH* gr, int vertex, bool* visited) {
    visited[vertex] = true;
    int count = 1;
    struct ADJ_NODE* tempNode = gr->adjList[vertex]->adjNode;
    while (tempNode != NULL) {
        if (!visited[tempNode->vertexNo - 1]) {
            count += dfs(gr, tempNode->vertexNo - 1, visited);
        }
        tempNode = tempNode->next;
    }
    return count;
}

struct INFLUENCER* findInfluencers(struct GRAPH* gr, int X, int Y) {
    struct INFLUENCER* head = NULL;
    for (int i = 0; i < gr->numOfVertices; i++) {
        if (gr->adjList[i]->inDegree >= X && gr->adjList[i]->total >= Y && gr->adjList[i]->deletedStatus == false) {
            struct INFLUENCER* newNode = malloc(sizeof(struct INFLUENCER));
            newNode->vertexId = gr->adjList[i]->vertexId;
            newNode->next = NULL;
            newNode->inDegree = gr->adjList[i]->inDegree;
            newNode->total = gr->adjList[i]->total;
            strcpy(newNode->name, gr->adjList[i]->name);
            strcpy(newNode->surname, gr->adjList[i]->surname);
            if (head == NULL) {
                head = newNode;
            } else {
                newNode->next = head;
                head = newNode;
            }
        }
    }
    printInfluencers(head);
    return head;
}

void printInfluencers(struct INFLUENCER* head) {
    printf("          -----------Influencers-----------\n");
    struct INFLUENCER* temp = head;
    while (temp != NULL) {
        printf("Id: %d Name: %s Surname: %s Indegree: %d Total Connections: %d\n", temp->vertexId, temp->name, temp->surname, temp->inDegree, temp->total);
        temp = temp->next;
    }
}
