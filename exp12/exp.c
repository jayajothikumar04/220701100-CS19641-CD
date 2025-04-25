#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

// Structure to represent a Three-Address Code instruction (simplified for this example)
typedef struct {
    char result[10];
    char operand1[10];
    char op[5];
    char operand2[10];
} TACInstruction;

// Function to perform a simple arithmetic operation if possible
int evaluate(int op1, char operator, int op2) {
    switch (operator) {
        case '+':
            return op1 + op2;
        case '-':
            return op1 - op2;
        case '*':
            return op1 * op2;
        case '/':
            if (op2 != 0) {
                return op1 / op2;
            }
            break;
        default:
            break;
    }
    return -1; // Indicate that evaluation was not possible
}

int main() {
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH];
    TACInstruction instruction;
    char tempResult[20];
    int instructionCount = 0;

    // Open the input file for reading
    inputFile = fopen("input.tac", "r");
    if (inputFile == NULL) {
        perror("Error opening input.tac");
        return 1;
    }

    // Open the output file for writing the optimized code
    outputFile = fopen("optimized.tac", "w");
    if (outputFile == NULL) {
        perror("Error opening optimized.tac");
        fclose(inputFile);
        return 1;
    }

    printf("Performing Copy Propagation (Simplified):\n");

    // A simple map to store variable copies (variable -> value)
    // In a real compiler, this would be more sophisticated.
    char copies[50][2][10]; // Stores up to 50 copies of (variable, value)
    int copyCount = 0;

    // Process each line (instruction) in the input TAC file
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        // Assume a simple TAC format: result = operand1 op operand2
        if (sscanf(line, "%s = %s %s %s", instruction.result, instruction.operand1, instruction.op, instruction.operand2) == 4) {
            int op1_val, op2_val, eval_result;
            int is_op1_const = 0;
            int is_op2_const = 0;

            // Check if operands are constants or variables with known constant values
            if (isdigit(instruction.operand1[0]) || (instruction.operand1[0] == '-' && isdigit(instruction.operand1[1]))) {
                op1_val = atoi(instruction.operand1);
                is_op1_const = 1;
            } else {
                // Check if operand1 has a known constant value from previous copy propagation
                for (int i = 0; i < copyCount; i++) {
                    if (strcmp(instruction.operand1, copies[i][0]) == 0) {
                        op1_val = atoi(copies[i][1]);
                        is_op1_const = 1;
                        break;
                    }
                }
            }

            if (isdigit(instruction.operand2[0]) || (instruction.operand2[0] == '-' && isdigit(instruction.operand2[1]))) {
                op2_val = atoi(instruction.operand2);
                is_op2_const = 1;
            } else {
                // Check if operand2 has a known constant value from previous copy propagation
                for (int i = 0; i < copyCount; i++) {
                    if (strcmp(instruction.operand2, copies[i][0]) == 0) {
                        op2_val = atoi(copies[i][1]);
                        is_op2_const = 1;
                        break;
                    }
                }
            }

            // Perform constant folding if both operands are constants
            if (is_op1_const && is_op2_const) {
                eval_result = evaluate(op1_val, instruction.op[0], op2_val);
                if (eval_result != -1) {
                    fprintf(outputFile, "%s = %d\n", instruction.result, eval_result);
                    printf("Constant folded: %s = %s %s %s  =>  %s = %d\n", instruction.result, instruction.operand1, instruction.op, instruction.operand2, instruction.result, eval_result);

                    // If the result is a constant, store it for potential copy propagation later
                    strcpy(copies[copyCount][0], instruction.result);
                    sprintf(copies[copyCount][1], "%d", eval_result);
                    copyCount++;
                    continue; // Skip writing the original instruction
                }
            }

            // Perform copy propagation: if an operand has a known constant value, use it
            if (is_op1_const) {
                fprintf(outputFile, "%s = %d %s %s\n", instruction.result, op1_val, instruction.op, instruction.operand2);
                printf("Copy propagated (operand1): %s = %s %s %s  =>  %s = %d %s %s\n", instruction.result, instruction.operand1, instruction.op, instruction.operand2, instruction.result, op1_val, instruction.op, instruction.operand2);
            } else if (is_op2_const) {
                fprintf(outputFile, "%s = %s %s %d\n", instruction.result, instruction.operand1, instruction.op, op2_val);
                printf("Copy propagated (operand2): %s = %s %s %s  =>  %s = %s %s %d\n", instruction.result, instruction.operand1, instruction.op, instruction.operand2, instruction.result, instruction.operand1, instruction.op, op2_val);
            } else {
                // No constant folding or direct copy propagation possible in this step
                fprintf(outputFile, "%s = %s %s %s\n", instruction.result, instruction.operand1, instruction.op, instruction.operand2);
            }

            // If the right-hand side is a simple copy (e.g., x = y), record it
            if (strcmp(instruction.op, "") == 0 && strcmp(instruction.operand2, "") == 0) {
                strcpy(copies[copyCount][0], instruction.result);
                strcpy(copies[copyCount][1], instruction.operand1);
                copyCount++;
                printf("Copy recorded: %s = %s\n", instruction.result, instruction.operand1);
            }
        } else if (sscanf(line, "%s = %s", instruction.result, instruction.operand1) == 2) {
            // Handle simple assignment (copy)
            fprintf(outputFile, "%s = %s\n", instruction.result, instruction.operand1);
            strcpy(copies[copyCount][0], instruction.result);
            strcpy(copies[copyCount][1], instruction.operand1);
            copyCount++;
            printf("Copy recorded: %s = %s\n", instruction.result, instruction.operand1);
        } else {
            // If the line doesn't match the expected TAC format, just write it as is
            fprintf(outputFile, "%s", line);
        }
        instructionCount++;
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("\nOptimized Three-Address Code written to optimized.tac\n");

    // Print the contents of the optimized file
    printf("\nContents of optimized.tac:\n");
    outputFile = fopen("optimized.tac", "r");
    if (outputFile != NULL) {
        char outputLine[MAX_LINE_LENGTH];
        while (fgets(outputLine, sizeof(outputLine), outputFile) != NULL) {
            printf("%s", outputLine);
        }
        fclose(outputFile);
    } else {
        perror("Error opening optimized.tac for reading");
    }

    return 0;
}