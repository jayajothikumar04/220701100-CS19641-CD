%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylex(void);
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at '%s': %s\n", yytext, s);
}

%}

%union {
    int num;
    char *str;
}

%token IF ELSE FOR WHILE SWITCH CASE DEFAULT BREAK CONTINUE RETURN
%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COLON COMMA
%token ARITH_OP REL_OP LOG_OP ASSIGN_OP
%token <num> NUMBER
%token <str> IDENTIFIER STRING

%%

program:
    | program control_structure
    ;

control_structure:
    if_statement
    | if_else_statement
    | if_else_if_statement
    | for_loop
    | while_loop
    | switch_statement
    ;

if_statement:
    IF LPAREN condition RPAREN statement_block
    { printf("Valid if statement\n"); }
    ;

if_else_statement:
    IF LPAREN condition RPAREN statement_block ELSE statement_block
    { printf("Valid if-else statement\n"); }
    ;

if_else_if_statement:
    IF LPAREN condition RPAREN statement_block else_if_list ELSE statement_block
    { printf("Valid if-else-if statement\n"); }
    ;

else_if_list:
    | else_if_list ELSE IF LPAREN condition RPAREN statement_block
    ;

for_loop:
    FOR LPAREN expression SEMICOLON condition SEMICOLON expression RPAREN statement_block
    { printf("Valid for loop\n"); }
    ;

while_loop:
    WHILE LPAREN condition RPAREN statement_block
    { printf("Valid while loop\n"); }
    ;

switch_statement:
    SWITCH LPAREN expression RPAREN LBRACE case_list default_case RBRACE
    { printf("Valid switch statement\n"); }
    ;

case_list:
    | case_list CASE expression COLON statements
    ;

default_case:
    | DEFAULT COLON statements
    ;

statement_block:
    LBRACE statements RBRACE
    | statement SEMICOLON
    ;

statements:
    | statements statement SEMICOLON
    ;

statement:
    expression
    | control_structure
    | BREAK SEMICOLON
    | CONTINUE SEMICOLON
    | RETURN SEMICOLON
    ;

condition:
    expression REL_OP expression
    | expression LOG_OP expression
    | expression
    ;

expression:
    IDENTIFIER
    | NUMBER
    | STRING
    | IDENTIFIER ASSIGN_OP expression
    | expression ARITH_OP expression
    | LPAREN expression RPAREN
    ;

%%

int main(int argc, char *argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening file");
            return 1;
        }
    }
    
    yyparse();
    
    if (argc > 1) {
        fclose(yyin);
    }
    
    return 0;
}