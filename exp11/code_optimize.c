#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

char tac[MAX][100];         // Store TAC lines
int n = 0;                  // Number of lines
int isDead[MAX];           // Flags for dead code

// Function to check if a variable is used again
int is_used_later(char var[], int index) {
    for (int i = index + 1; i < n; i++) {
        if (strstr(tac[i], var) != NULL) {
            return 1;
        }
    }
    return 0;
}

// Function to extract left-hand side variable
void get_lhs(char* line, char* lhs) {
    int i = 0;
    while (line[i] != '=' && line[i] != '\0') {
        lhs[i] = line[i];
        i++;
    }
    lhs[i] = '\0';
}

// Dead Code Elimination
void dead_code_elimination() {
    char lhs[20];
    for (int i = 0; i < n; i++) {
        get_lhs(tac[i], lhs);
        if (!is_used_later(lhs, i)) {
            isDead[i] = 1;
        }
    }
}

// Common Subexpression Elimination
void common_subexpression_elimination() {
    for (int i = 0; i < n - 1; i++) {
        if (isDead[i]) continue;

        char* expr1 = strchr(tac[i], '=') + 1;
        for (int j = i + 1; j < n; j++) {
            if (isDead[j]) continue;

            char* expr2 = strchr(tac[j], '=') + 1;
            if (strcmp(expr1, expr2) == 0) {
                isDead[j] = 1;
                printf("CSE: Eliminating redundant expression: %s", tac[j]);
            }
        }
    }
}

// Display original code
void display_original_code() {
    printf("\nOriginal Code:\n");
    for (int i = 0; i < n; i++) {
        printf("%s", tac[i]);
    }
}

// Display optimized code
void display_optimized_code() {
    printf("\nOptimized Code:\n");
    for (int i = 0; i < n; i++) {
        if (!isDead[i]) {
            printf("%s", tac[i]);
        }
    }
}

int main() {
    FILE* fp;
    char filename[] = "input_tac.txt";

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    while (fgets(tac[n], sizeof(tac[n]), fp)) {
        isDead[n] = 0;
        n++;
    }
    fclose(fp);

    display_original_code();                  // Show input before optimization

    dead_code_elimination();                 // Apply DCE
    common_subexpression_elimination();      // Apply CSE

    display_optimized_code();                // Show optimized result
    return 0;
}
