%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 0;
char temp_var[100];

char* new_temp() {
    sprintf(temp_var, "t%d", temp_count++);
    return strdup(temp_var);
}

void yyerror(const char *s);
int yylex();
%}

%union {
    int ival;
    char *sval;
}

%token <sval> ID
%token <ival> NUMBER
%token ASSIGN PLUS MINUS MUL DIV LPAREN RPAREN SEMICOLON

%type <sval> expr statement

%%

program:
    statement SEMICOLON        { printf("\n✅ TAC Generation Complete\n"); }
;

statement:
    ID ASSIGN expr             {
                                printf("%s = %s\n", $1, $3);
                              }
;

expr:
    expr PLUS expr             {
                                char *t = new_temp();
                                printf("%s = %s + %s\n", t, $1, $3);
                                $$ = t;
                              }
  | expr MINUS expr            {
                                char *t = new_temp();
                                printf("%s = %s - %s\n", t, $1, $3);
                                $$ = t;
                              }
  | expr MUL expr              {
                                char *t = new_temp();
                                printf("%s = %s * %s\n", t, $1, $3);
                                $$ = t;
                              }
  | expr DIV expr              {
                                char *t = new_temp();
                                printf("%s = %s / %s\n", t, $1, $3);
                                $$ = t;
                              }
  | NUMBER                     {
                                char str[20];
                                sprintf(str, "%d", $1);
                                $$ = strdup(str);
                              }
  | ID                         { $$ = $1; }
  | LPAREN expr RPAREN         { $$ = $2; }
;

%%

void yyerror(const char *s) {
    printf("❌ Error: %s\n", s);
}

int main() {
    printf("Enter expression (e.g., a = b + c * d;):\n");
    yyparse();
    return 0;
}
