%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER
%left '+'
%left '*'

%%
input:
      expr { printf("Result = %d\n", $1); return 0; }
    ;

expr:
      expr '+' expr   { $$ = $1 + $3; }
    | expr '*' expr   { $$ = $1 * $3; }
    | NUMBER          { $$ = $1; }
    ;
%%

int main() {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Syntax Error: %s\n", s);
}
