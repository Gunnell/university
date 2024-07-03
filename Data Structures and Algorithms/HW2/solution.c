#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME 15

typedef struct node {
    char name[NAME];
    struct node* sibling;
    struct node* child;
    struct node* parent; 
} NODE;

void mkdirr(NODE* cursor, char* name);
void dir(NODE* cursor, int level);
void chgdir(NODE** cursor, char* name);
void rmvdir(NODE* cursor, char* name);
int count(NODE* cursor);

int main() {
    NODE* root = (NODE*)malloc(sizeof(NODE));
    strcpy(root->name, "root");
    root->child = NULL;
    root->sibling = NULL;
    root->parent = NULL; 
    NODE* cursor = root;
   
    printf("To terminate the processes please type in 'exit'\n");
    char command[30];
    char name[NAME];

    while (1) {
        printf("Please enter the command\n");
        scanf("%[^\n]%*c", command);

        char *saveptr;
        char *cmd = strtok_r(command, " ", &saveptr);
        if (cmd == NULL) continue;

        if (strcmp(cmd, "exit") == 0) {
            printf("Saving session...\n...copying shared history...\n...saving history...truncating history files...\n...completed.\n\n[Process completed]\n");
            break;
        }

        char *arg = strtok_r(NULL, " ", &saveptr);
        if (arg != NULL) {
            strcpy(name, arg);
        } else {
            name[0] = '\0';
        }

        if (strcmp(cmd, "mkdir") == 0) {
            if (name[0] != '\0') {
                mkdirr(cursor, name);
            } else {
                printf("Please provide a directory name.\n");
            }
        } else if (strcmp(cmd, "chgdir") == 0) {
            if (name[0] != '\0') {
                chgdir(&cursor, name);
            } else {
                printf("Please provide a directory name.\n");
            }
        } else if (strcmp(cmd, "rmvdir") == 0) {
            if (name[0] != '\0') {
                rmvdir(cursor, name);
            } else {
                printf("Please provide a directory name.\n");
            }
        } else if (strcmp(cmd, "dir") == 0) {
            dir(cursor, 0);
        } else if (strcmp(cmd, "count") == 0) {
            printf("Number of directories: %d\n", count(cursor));
        } else {
            printf("Please enter a valid command!\n");
        }
    }

    free(root);
    return 0;
}

void mkdirr(NODE* cursor, char* name) {
    // Check for duplicate directories
    NODE* tmp = cursor->child;
    while (tmp != NULL) {
        if (strcmp(tmp->name, name) == 0) {
            printf("Directory already exists\n");
            return;
        }
        tmp = tmp->sibling;
    }

    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->child = NULL;
    node->sibling = NULL;
    node->parent = cursor; 
    strcpy(node->name, name);

    if (cursor->child == NULL) {
        cursor->child = node;
    } else {
        tmp = cursor->child;
        while (tmp->sibling != NULL) {
            tmp = tmp->sibling;
        }
        tmp->sibling = node;
    }
}

void dir(NODE* cursor, int level) {
    if (cursor->child == NULL) {
        printf("No directories exist\n");
        return;
    }
    NODE* tmp = cursor->child;
    while (tmp != NULL) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("%s\n", tmp->name);
        if (tmp->child != NULL) {
            dir(tmp, level + 1);
        }
        tmp = tmp->sibling;
    }
}

void chgdir(NODE** cursor, char* name) {
    if (strcmp(name, "..") == 0) {
        if ((*cursor)->parent != NULL) {
            *cursor = (*cursor)->parent;
        } else {
            printf("Already at the root directory\n");
        }
    } else {
        NODE* tmp = (*cursor)->child;
        while (tmp != NULL && strcmp(tmp->name, name) != 0) {
            tmp = tmp->sibling;
        }
        if (tmp != NULL) {
            *cursor = tmp;
        } else {
            printf("Directory not found\n");
        }
    }
}

void rmvdir(NODE* cursor, char* name) {
    NODE* tmp = cursor->child;
    NODE* prev = NULL;
    while (tmp != NULL && strcmp(tmp->name, name) != 0) {
        prev = tmp;
        tmp = tmp->sibling;
    }
    if (tmp != NULL) {
        if (tmp->child != NULL) {
            printf("Cannot delete folder. The directory is not empty.\n");
            return;
        }
        if (prev == NULL) {
            cursor->child = tmp->sibling;
        } else {
            prev->sibling = tmp->sibling;
        }
        free(tmp);
    } else {
        printf("The directory does not exist\n");
    }
}

int count(NODE* cursor) {
    int count = 0;
    NODE* tmp = cursor->child;
    while (tmp != NULL) {
        count++;
        tmp = tmp->sibling;
    }
    return count;
}
