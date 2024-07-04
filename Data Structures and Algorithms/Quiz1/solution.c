
// Time Complexity: O(N * M)
// Space Complexity: O(K/3)=O(K)
#include <stdio.h>
#include <stdlib.h>

typedef struct HashTableEntry {
    int color;
    int count;
    struct HashTableEntry* next;
} HashTableEntry;

#define TABLE_SIZE 1024

unsigned int hash(int color) {
    return color % TABLE_SIZE;
}

void insert(HashTableEntry* table[], int color) {
    unsigned int index = hash(color);
    HashTableEntry* entry = table[index];
    
    while (entry != NULL) {
        if (entry->color == color) {
            entry->count++;
            return;
        }
        entry = entry->next;
    }
    
    entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    entry->color = color;
    entry->count = 1;
    entry->next = table[index];
    table[index] = entry;
}

void print_histogram(HashTableEntry* table[]) {
    printf("| Color | Count |\n");
    printf("|-------|-------|\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashTableEntry* entry = table[i];
        while (entry != NULL) {
            printf("| %5d | %5d |\n", entry->color, entry->count);
            entry = entry->next;
        }
    }
}


int count_distinct_colors(HashTableEntry* table[]) {
    int distinctColorCount = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashTableEntry* entry = table[i];
        while (entry != NULL) {
            distinctColorCount++;
            entry = entry->next;
        }
    }
    return distinctColorCount;
}

int main() {
    int N, M, K;
    printf("Matrisin N ve M degerleri\n");
    scanf("%d", &N);
    scanf("%d", &M);
    printf("K degeri\n");
    scanf("%d", &K);
    
    int img[N][M];
    printf("Resim matrisini giriniz, renk degerini girerken 0-K degerine dikkat ediniz\n");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            scanf("%d", &img[i][j]);
        }
    }
    

    HashTableEntry* table[TABLE_SIZE] = {0};
    
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            insert(table, img[i][j]);
        }
    }
    
    
    int distinctColorCount = count_distinct_colors(table);
    printf("Farklı Renk Sayısı: %d\n", distinctColorCount);
    
    
    print_histogram(table);
    
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashTableEntry* entry = table[i];
        while (entry != NULL) {
            HashTableEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    
    return 0;
}
