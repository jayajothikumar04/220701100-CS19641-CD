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
    while (isspace(str[0])) str++;
    for (i = strlen(str) - 1; i >= 0 && isspace(str[i]); i--) str[i] = '\0';
}

// Function to extract TAC components
int parse_tac(char *line, char *dest, char *src1, char *op, char *src2) {
    char *equal_sign = strchr(line, '=');
    if (!equal_sign) return 0; // No '=' found, invalid TAC

    // Extract destination variable
    strncpy(dest, line, equal_sign - line);
    dest[equal_sign - line] = '\0';
    trim(dest);

    // Find the operator (+, -, *, /)
    char *operator_pos = strpbrk(equal_sign + 1, "+-*/");
    if (!operator_pos) return 0; // No operator found

    // Extract operands
    strncpy(src1, equal_sign + 1, operator_pos - (equal_sign + 1));
    src1[operator_pos - (equal_sign + 1)] = '\0';
    trim(src1);

    strncpy(op, operator_pos, 1);
    op[1] = '\0';

    strcpy(src2, operator_pos + 1);
    trim(src2);

    return 1; // Successfully parsed
}

// Generate 8086 code for one TAC line
void generate_8086(char *line) {
    char dest[10], src1[10], src2[10], op[2];

    printf("DEBUG: Processing line - '%s'\n", line); 

    if (!parse_tac(line, dest, src1, op, src2)) {
        printf("DEBUG: Parsing failed for line '%s'\n", line);
        printf("; Unsupported instruction: %s\n", line);
        return;
    }

    printf("DEBUG: Parsed -> dest: '%s', src1: '%s', op: '%s', src2: '%s'\n", dest, src1, op, src2);

    if (strcmp(op, "+") == 0) {
        printf("MOV %s, [%s]\n", REG_AX, src1);
        printf("ADD %s, [%s]\n", REG_AX, src2);
        printf("MOV [%s], %s\n", dest, REG_AX);
    } else if (strcmp(op, "-") == 0) {
        printf("MOV %s, [%s]\n", REG_AX, src1);
        printf("SUB %s, [%s]\n", REG_AX, src2);
        printf("MOV [%s], %s\n", dest, REG_AX);
    } else if (strcmp(op, "*") == 0) {
        printf("MOV %s, [%s]\n", REG_AX, src1);
        printf("MOV %s, [%s]\n", REG_BX, src2);
        printf("MUL %s\n", REG_BX);
        printf("MOV [%s], %s\n", dest, REG_AX);
    } else if (strcmp(op, "/") == 0) {
        printf("MOV %s, [%s]\n", REG_AX, src1);
        printf("MOV %s, 0\n", REG_DX); // Clear DX
        printf("MOV %s, [%s]\n", REG_BX, src2);
        printf("DIV %s\n", REG_BX);
        printf("MOV [%s], %s\n", dest, REG_AX);
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
