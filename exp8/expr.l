%{
#include "expr.tab.h"
#include <string.h>
#ifndef strdup
#define strdup _strdup  // For compatibility with Windows/Mingw
#endif
%}

%%

[ \t\n]                             ; // Skip whitespace
[a-zA-Z_][a-zA-Z0-9_]*              { yylval.sval = strdup(yytext); return ID; }
[0-9]+                              { yylval.ival = atoi(yytext); return NUMBER; }
"="                                 { return ASSIGN; }
"("                                 { return LPAREN; }
")"                                 { return RPAREN; }
"+"                                 { return PLUS; }
"-"                                 { return MINUS; }
"*"                                 { return MUL; }
"/"                                 { return DIV; }
";"                                 { return SEMICOLON; }

.                                   { return yytext[0]; }

%%

int yywrap() { return 1; }
