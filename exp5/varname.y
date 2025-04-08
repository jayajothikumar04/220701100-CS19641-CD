%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(const char *s);
%}

%token VALID INVALID

%%
input:
    VALID   { printf("Valid variable name\n"); }
  | INVALID { printf("Invalid variable name\n"); }
  ;
%%

int main() {
    printf("Enter a variable name: ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    // Remove this line if you don't want the extra error:
    // printf("Syntax Error: %s\n", s);
}
