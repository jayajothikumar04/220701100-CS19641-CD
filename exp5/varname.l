%{
#include "varname.tab.h"
%}

%%
[a-zA-Z][a-zA-Z0-9]*    { return VALID; }
.|\n                    { return INVALID; }
%%

int yywrap() {
    return 1;
}
