#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

typedef struct {
    char item[MAX];
    int Sptr;
} STACK;

void init(STACK* s) {
    s->Sptr = 0;
}

int isEmpty(STACK* s) {
    return s->Sptr == 0;
}

int isFull(STACK* s) {
    return s->Sptr == MAX;
}

int push(STACK* s, char x) {
    if (!isFull(s)) {
        s->item[s->Sptr++] = x;
        return 1;
    }
    return 0;
}

int pop(STACK* s) {
    if (!isEmpty(s)) {
        --s->Sptr;
        return 1;
    }
    return 0;
}

char* processCode(char* code) {
    STACK s;
    init(&s);
    int len = strlen(code);

    for (int i = 0; i < len; i++) {
        if (isalpha(code[i])) {
            push(&s, code[i]);
        } else if (isdigit(code[i])) {
            int num = code[i] - '0';
            if (s.Sptr < num) {
                printf("Error: Not enough characters to delete\n");
                return NULL;
            }
            for (int j = 0; j < num; j++) {
                pop(&s);
            }
        }
    }

    char* result = (char*)malloc((s.Sptr + 1) * sizeof(char));
    for (int i = 0; i < s.Sptr; i++) {
        result[i] = s.item[i];
    }
    result[s.Sptr] = '\0';

    return result;
}

int main() {
    char code1[MAX], code2[MAX];

    printf("Enter the first code: ");
    scanf("%s", code1);
    printf("Enter the second code: ");
    scanf("%s", code2);

    char* key1 = processCode(code1);
    char* key2 = processCode(code2);

    if (key1 && key2 && strcmp(key1, key2) == 0) {
        printf("Connected\n");
    } else {
        printf("Connection denied\n");
    }

    free(key1);
    free(key2);

    return 0;
}
