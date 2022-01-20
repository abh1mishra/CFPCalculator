/* Do Not Modify This File */
%{
#include <stdio.h>
#include <stdlib.h>
#include "fp_parse.h"

float fp_val;
int lineno = 1;

%}
%%
print		         {return PRINT;}
display          {return DISPLAY;}
\=		           {return ASSIGN;}
\+	 	           {return PLUS;}
\*		           {return MULT;}
[a-z]		         {return (yytext[0]-'a')+1;}
[-]?[0-9]*		       {fp_val = atof(yytext); return FLOAT;}
[-]?[0-9]*\.[0-9]*	 {fp_val = atof(yytext); return FLOAT;}
[ \t]		;
[\n]		         {lineno++; return EOLN;}
.		             {printf("Illegal input: %s\n",yytext); exit(2);}
%%
