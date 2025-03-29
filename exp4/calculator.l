%{
#include <stdio.h>
#include <stdlib.h>

int paren_count = 0;
int error_flag = 0;
%}

DIGIT       [0-9]
FLOAT       {DIGIT}+\.{DIGIT}+
INTEGER     {DIGIT}+
NUMBER      ({FLOAT}|{INTEGER})

%%

[ \t\n]+               ; // Ignore whitespace

{NUMBER}               { printf("NUMBER: %s\n", yytext); }

"+"                    { printf("PLUS\n"); }
"-"                    { printf("MINUS\n"); }
"*"                    { printf("MULTIPLY\n"); }
"/"                    { printf("DIVIDE\n"); }

"("                    { printf("LEFT_PAREN\n"); paren_count++; }
")"                    {
                          printf("RIGHT_PAREN\n");
                          if (paren_count == 0) {
                              printf("Unmatched right parenthesis.\n");
                              error_flag = 1;
                          } else {
                              paren_count--;
                          }
                       }

.                      { printf("Invalid character: %s\n", yytext); error_flag = 1; }

%%

int main() {
    printf("Enter an arithmetic expression:\n");
    yylex();

    if (paren_count != 0) {
        printf("Error: Unmatched parentheses.\n");
        error_flag = 1;
    }

    if (!error_flag)
        printf("Valid arithmetic expression.\n");

    return 0;
}

int yywrap() {
    return 1;
}
