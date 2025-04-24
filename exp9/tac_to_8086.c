#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 100
#define REG_AX "AX"
#define REG_BX "BX"
#define REG_DX "DX"

// Function to trim leading/trailing whitespace
void trim(char *str) {
    int i;
    while(isspace(str[0])) str++;
    for(i = strlen(str) - 1; i >= 0 && isspace(str[i]); i--) str[i] = '\0';
}

// Function to check if a string is a temporary variable (like t0, t1)
int is_temp(char *str) {
    return str[0] == 't' && isdigit(str[1]);
}

// Generate 8086 code for one TAC line
void generate_8086(char *line) {
    char dest[10], src1[10], src2[10], op;

    // Check if it's a simple assignment: a = b
    if (sscanf(line, "%s = %s", dest, src1) == 2) {
        printf("MOV %s, [%s]\n", REG_AX, src1);
        printf("MOV [%s], %s\n", dest, REG_AX);
        return;
    }

    // Check for arithmetic operation: t1 = t0 + d
    if (sscanf(line, "%s = %[^+*/-] %c %s", dest, src1, &op, src2) == 4) {
        trim(src1);
        trim(src2);

        if (op == '+') {
            printf("MOV %s, [%s]\n", REG_AX, src1);
            printf("ADD %s, [%s]\n", REG_AX, src2);
            printf("MOV [%s], %s\n", dest, REG_AX);
        } else if (op == '-') {
            printf("MOV %s, [%s]\n", REG_AX, src1);
            printf("SUB %s, [%s]\n", REG_AX, src2);
            printf("MOV [%s], %s\n", dest, REG_AX);
        } else if (op == '*') {
            printf("MOV %s, [%s]\n", REG_AX, src1);
            printf("MOV %s, [%s]\n", REG_BX, src2);
            printf("MUL %s\n", REG_BX);
            printf("MOV [%s], %s\n", dest, REG_AX);
        } else if (op == '/') {
            printf("MOV %s, [%s]\n", REG_AX, src1);
            printf("MOV %s, 0\n", REG_DX); // Clear DX
            printf("MOV %s, [%s]\n", REG_BX, src2);
            printf("DIV %s\n", REG_BX);
            printf("MOV [%s], %s\n", dest, REG_AX);
        }
    } else {
        printf("; Unsupported instruction: %s\n", line);
    }
}

int main() {
    char line[MAX_LINE];

    printf("Enter TAC line by line (Ctrl+D to stop):\n");
    while (fgets(line, sizeof(line), stdin)) {
        trim(line);
        if (strlen(line) == 0) continue;
        generate_8086(line);
        printf("\n");
    }

    return 0;
}
