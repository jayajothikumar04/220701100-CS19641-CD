#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int tempCount = 0;

// Stack structure for operator precedence
char stack[MAX];
int top = -1;

// For holding postfix expression
char postfix[MAX][MAX];
int pIndex = 0;

// For output TAC
void generateTAC(char tokens[][MAX], int count) {
    char temp[MAX];
    char result[MAX][MAX];
    int tIndex = 0;

    for (int i = 2; i < count; i++) {
        if (strcmp(tokens[i], "*") == 0 || strcmp(tokens[i], "/") == 0 ||
            strcmp(tokens[i], "+") == 0 || strcmp(tokens[i], "-") == 0) {
            char t[10];
            sprintf(t, "t%d", tempCount++);
            printf("%s = %s %s %s\n", t, tokens[i - 1], tokens[i], tokens[i + 1]);
            strcpy(tokens[i + 1], t);
            i++;
        }
    }

    // Final assignment
    printf("%s = %s\n", tokens[0], tokens[count - 1]);
}

int main() {
    FILE *fp;
    char line[MAX];
    char tokens[MAX][MAX];
    int tCount;

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    printf("Generating TAC for expressions in input.txt...\n\n");

    while (fgets(line, MAX, fp)) {
        tCount = 0;

        // Tokenize line
        char *token = strtok(line, " \n");
        while (token != NULL) {
            strcpy(tokens[tCount++], token);
            token = strtok(NULL, " \n");
        }

        // Generate TAC for current expression
        generateTAC(tokens, tCount);
        printf("\n");
    }

    fclose(fp);
    return 0;
}
