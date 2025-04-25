%{
#include "control_yacc.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void yyerror(const char *);
%}

%%

"if"            { return IF; }
"else"          { return ELSE; }
"for"           { return FOR; }
"while"         { return WHILE; }
"switch"        { return SWITCH; }
"case"          { return CASE; }
"default"       { return DEFAULT; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"return"        { return RETURN; }

"{"             { return LBRACE; }
"}"             { return RBRACE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }
":"             { return COLON; }
","             { return COMMA; }

"+"|"-"|"*"|"/"|"%"|"++"|"--" { return ARITH_OP; }
"<"|">"|"<="|">="|"=="|"!="   { return REL_OP; }
"&&"|"||"|"!"   { return LOG_OP; }
"="|"+="|"-="|"*="|"/="|"%="  { return ASSIGN_OP; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
\"([^\\\"]|\\.)*\" { yylval.str = strdup(yytext); return STRING; }

[ \t\n]         ; /* skip whitespace */
.               { printf("Invalid character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}