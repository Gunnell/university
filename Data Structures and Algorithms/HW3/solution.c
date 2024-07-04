#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEIGHBORS 10
#define MAX_QUERY_LEN 100

typedef struct Node {
    char queryString[MAX_QUERY_LEN];
    int queryCount;
    struct Node *neighbors[MAX_NEIGHBORS];
    int neighborCount;
} Node;

Node* createNode(const char* queryString) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->queryString, queryString);
    newNode->queryCount = 0;
    newNode->neighborCount = 0;
    return newNode;
}

void addNeighbor(Node* node, Node* neighbor) {
    for (int i = 0; i < node->neighborCount; i++) {
        if (strcmp(node->neighbors[i]->queryString, neighbor->queryString) == 0) {
            return; // Neighbor already exists
        }
    }
    if (node->neighborCount < MAX_NEIGHBORS) {
        node->neighbors[node->neighborCount++] = neighbor;
    }
}

Node* findNode(Node* graph[], int size, const char* queryString) {
    for (int i = 0; i < size; i++) {
        if (strcmp(graph[i]->queryString, queryString) == 0) {
            return graph[i];
        }
    }
    return NULL;
}

void mergeNeighbors(Node* target, Node* source) {
    for (int i = 0; i < source->neighborCount; i++) {
        addNeighbor(target, source->neighbors[i]);
    }
}

void mergeGraphs(Node* graph1[], int size1, Node* graph2[], int size2, Node* result[], int* resultSize) {
    for (int i = 0; i < size1; i++) {
        result[(*resultSize)++] = graph1[i];
    }
    for (int i = 0; i < size2; i++) {
        Node* existingNode = findNode(result, *resultSize, graph2[i]->queryString);
        if (existingNode) {
            mergeNeighbors(existingNode, graph2[i]);
        } else {
            result[(*resultSize)++] = graph2[i];
        }
    }
}

int compareQueryCount(const void* a, const void* b) {
    Node* nodeA = *(Node**)a;
    Node* nodeB = *(Node**)b;
    return nodeB->queryCount - nodeA->queryCount;
}

void queryGraph(Node* graph[], int size, const char* queryString) {
    Node* node = findNode(graph, size, queryString);
    if (node) {
        node->queryCount++;
        printf("Top 3 neighbors of %s:\n", queryString);

        Node* sortedNeighbors[MAX_NEIGHBORS];
        memcpy(sortedNeighbors, node->neighbors, sizeof(Node*) * node->neighborCount);

        qsort(sortedNeighbors, node->neighborCount, sizeof(Node*), compareQueryCount);

        for (int i = 0; i < node->neighborCount && i < 3; i++) {
            printf("%s (Queried %d times)\n", sortedNeighbors[i]->queryString, sortedNeighbors[i]->queryCount);
        }
    } else {
        printf("Node %s not found in graph.\n", queryString);
    }
}

int main() {
    Node* graph1[10];
    Node* graph2[10];
    Node* mergedGraph[20];
    int size1 = 0, size2 = 0, mergedSize = 0;

  
    Node* A = createNode("A");
    Node* B = createNode("B");
    Node* C = createNode("C");
    Node* D = createNode("D");
    addNeighbor(A, B);
    addNeighbor(A, C);
    addNeighbor(A, D);
    graph1[size1++] = A;
    graph1[size1++] = B;
    graph1[size1++] = C;
    graph1[size1++] = D;

    Node* F = createNode("F");
    Node* H = createNode("H");
    Node* I = createNode("I");
    addNeighbor(F, B);
    addNeighbor(F, C);
    addNeighbor(F, H);
    addNeighbor(F, I);
    graph2[size2++] = F;
    graph2[size2++] = B;
    graph2[size2++] = C;
    graph2[size2++] = H;
    graph2[size2++] = I;

    
    mergeGraphs(graph1, size1, graph2, size2, mergedGraph, &mergedSize);

    
    char query[MAX_QUERY_LEN];
    while (1) {
        printf("Enter query string (or 'exit' to quit): ");
        scanf("%s", query);
        if (strcmp(query, "exit") == 0) {
            break;
        }
        queryGraph(mergedGraph, mergedSize, query);
    }

  
    for (int i = 0; i < mergedSize; i++) {
        free(mergedGraph[i]);
    }

    return 0;
}
