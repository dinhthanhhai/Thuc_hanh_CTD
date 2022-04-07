#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000

typedef struct node {
    char string[MAX_SIZE];
    int count;
    char lines[MAX_SIZE];
    struct node *next;
} node;

node *head = NULL, *stopw = NULL;

node *sortedInsert(node *head_t, node *newNode) {
    if (head_t == NULL ||
        strcmp(newNode->string, head_t->string) < 0) { 
        newNode->next = head_t;                                         
        head_t = newNode;
    } else {
        node *current = head_t, *previous; 
        while (current->next != NULL && strcmp(newNode->string, current->string) > 0) { 
            previous = current;                     
            current = current->next;
        }
        if (current->next == NULL &&
            strcmp(newNode->string, current->string) > 0) {
            current->next = newNode;        
        } else {
            newNode->next = current;        
            previous->next = newNode;
        }
    }
    return head_t;
}


node *newAndInsert(node *head_t, char str[], int line) {
    node *temp = head_t;
    int check = 0;
    while (temp != NULL) {
        if (!strcmp(str, temp->string)) {
            check = 1;
            break;
        }
        temp = temp->next;
    }
    if (check == 0) { 
        node *newNode = (node *) malloc(sizeof(node));
        strcpy(newNode->string, str);
        newNode->count = 1;
        sprintf(newNode->lines, " %d", line);
        newNode->next = NULL;
        head_t = sortedInsert(head_t, newNode);
        return head_t;
    } else {
        
        temp->count++;
        char s[10] = "";
        sprintf(s, " %d", line); 
        
        for (int i = (int) strlen(s) - 1; i != 0; --i) {
            if (s[i] != temp->lines[strlen(temp->lines) + i - strlen(s)]) {
                sprintf(temp->lines, "%s, %d", temp->lines, line); 
            }
        }
        return head_t;
    }
}


void printList(node *head_t) {
    node *temp = head_t;
    while (temp != NULL) {
        printf("%s %d,%s\n", temp->string, temp->count, temp->lines);
        temp = temp->next;
    }
}

void freeList(node *head_t) {
    node *temp;
    while (head_t != NULL) {
        temp = head_t;
        head_t = head_t->next;
        free(temp);
    }
}

void readFileStopw() {
    FILE *fptr = fopen("stopw.txt", "r");
    if (fptr != NULL) {
        while (!feof(fptr)) {
            char s[100] = "";
            fscanf(fptr, "%s", s);
            stopw = newAndInsert(stopw, s, 0);
        }
    }
}

//kiem tra tu 
int check(char str[]) {
    node *temp = stopw;
    while (temp != NULL) {
        if (!strcmp(str, temp->string))
            return 1;
        temp = temp->next;
    }
    return 0;
}


void readFileText() {
    FILE *fptr = fopen("vanban.txt", "r");
    int countLine = 1, sign = 1; 
    if (fptr != NULL) {
        while (!feof(fptr)) {
            char s[100] = ""; 
            int c = 0, curLine = countLine, vietHoa = 0;
          
            do {
                if (strlen(s) == 0 && c != tolower(c))  
                    vietHoa = 1;                               
                sprintf(s, "%s%c", s, tolower(c));
                c = fgetc(fptr); 
                if (c == '\n')      
                    countLine++;
            } while (isalpha(c));
           
            if (strlen(s) != 0 && !check(s) && ((vietHoa && sign) || !vietHoa)) {
                head = newAndInsert(head, s, curLine); 
                if (sign) 
                    sign = 0;
            }
            if (c == '.') 
                sign = 1;
        }
    }
}

int main() {
    readFileStopw();
    readFileText();
    printList(head);
    freeList(head);
    freeList(stopw);
    return 0;
}