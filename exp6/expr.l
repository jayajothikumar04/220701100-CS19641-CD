%{
#include "expr.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+  {
            yylval = atoi(yytext); 
            return NUMBER; 
        }
[+*]    return *yytext;
[\n]    return 0;
[ \t]   ; // Ignore whitespace
.       return yytext[0];
%%

int yywrap() {
    return 1;
}
