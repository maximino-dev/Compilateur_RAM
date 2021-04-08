/* 
  BOGADO GARCIA Maximino
  L3 Informatique

    lexer.lex
*/
%{
#include <string.h>
#include "parser.h" 
%}
  
%option nounput
%option noinput

NOMBRE  0|[1-9][0-9]*
ID [a-zA-Z][a-zA-Z0-9]*
COMMENTAIRE #.*\n
%%

{COMMENTAIRE}  {/* ne rien faire */}
"<-"           {return AFFECT;}
[%)(*/+-]      {return yytext[0];}
[<>=]          {return yytext[0];}
"<="           {return EGAL_P;}
">="		   {return EGAL_G;}
"!="           {return NON_EGAL;}
"OU"           {return OU;}
"ET"           {return ET;}
"SI"           {return SI;}
"ALORS"        {return ALORS;}
"SINON"        {return SINON;}
"FSI"          {return FSI;}
"NON"          {return NON;}
"DEBUT"        {return DEBUT;}
"VAR"          {return VAR;}
"FAIRE"        {return FAIRE;}
"FTQ"          {return FTQ;}
"FIN"          {return FIN;}
"TQ"           {return TQ;}
"LIRE"         {return LIRE;}
"AFFICHER"     {return AFFICHER;}
"\n"           {return NL;}
{ID}           {strcpy(yylval.id,yytext);return ID;}
{NOMBRE}       {yylval.nb = atoi(yytext); return NB;}
[ \t]          {}
.              {fprintf(stderr, "[err lexer] caractere inconnu %c\n",yytext[0]); return 1;}

%%
