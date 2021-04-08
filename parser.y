/* 
  BOGADO GARCIA Maximino
  L3 Informatique

    parser.y
*/
%{
  #include <stdio.h>
  #include <ctype.h>
  #include <unistd.h>
  
  #include "asa.h"
  #include "ts.h"

  extern int yylex();

  extern FILE *fichier_ram;
  
%}

%union{
  int nb;
  struct asa * noeud;
  char id[64];
 };

%define parse.error verbose

%token <nb> NB DEBUT FIN NL VAR <noeud> AFFECT <id> ID FAIRE TQ FTQ EGAL_P EGAL_G NON_EGAL ET OU NON SI SINON FSI ALORS AFFICHER LIRE

%type <noeud> EXP INST LVAL INSTS BOOL

%right AFFECT
%left '+' '-' OU
%left '/' '*' '%' ET
%left NEG NON

%start PROG

%%

PROG : DEBUT NL INSTS FIN NL  { codegen($3); fprintf(fichier_ram,"STOP\n"); free_asa($3);}
| DEBUT NL INSTS FIN          { codegen($3); fprintf(fichier_ram,"STOP\n"); free_asa($3);}
;

INSTS: INST                { $$ = $1; }
| INST INSTS               { $$ = creer_noeudInst($1, $2);}
;

INST: EXP NL               { $$ = $1;}
| TQ BOOL FAIRE NL
  INSTS
  FTQ NL                   { $$ = creer_noeudTq($2,$5);}
| SI BOOL ALORS NL
  INSTS
  SINON NL
  INSTS
  FSI NL                   { $$ = creer_noeudSi($2,$5,$8);}
| VAR LVAL NL              { $$ = creer_noeudDecl($2,1);}
| LVAL AFFECT EXP NL       { $$ = creer_noeudAffect($1,$3);}
| LVAL AFFECT BOOL NL      { $$ = creer_noeudAffect($1,$3);}
| AFFICHER BOOL NL         { $$ = creer_noeudAfficher($2);}
| AFFICHER EXP NL          { $$ = creer_noeudAfficher($2);}
| LIRE LVAL NL             { $$ = creer_noeudLire($2);}
;

BOOL: '(' BOOL ')'         { $$ = $2;}
| NON BOOL                 { $$ = creer_noeudNon($2);}
| BOOL OU BOOL             { $$ = creer_noeudBool("OU",$1,$3);}
| BOOL ET BOOL             { $$ = creer_noeudBool("ET",$1,$3);}
| EXP '<' EXP              { $$ = creer_noeudComp("<",$1,$3);}
| EXP '>' EXP              { $$ = creer_noeudComp(">",$1,$3);}
| EXP '=' EXP              { $$ = creer_noeudComp("=",$1,$3);}
| EXP NON_EGAL EXP         { $$ = creer_noeudComp("!=",$1,$3);}
| EXP EGAL_P EXP           { $$ = creer_noeudComp("<=",$1,$3);}
| EXP EGAL_G EXP           { $$ = creer_noeudComp(">=",$1,$3);}
;

LVAL: ID                   { $$ = creer_feuilleId($1); };

EXP: NB                    { $$ = creer_feuilleNb(yylval.nb); }
| '(' EXP ')'              { $$ = $2; }
| EXP '*' EXP              { $$ = creer_noeudOp('*', $1, $3); }
| EXP '-' EXP              { $$ = creer_noeudOp('-', $1, $3); }
| EXP '/' EXP              { $$ = creer_noeudOp('/', $1, $3); }
| EXP '+' EXP              { $$ = creer_noeudOp('+', $1, $3); }
| EXP '%' EXP              { $$ = creer_noeudOp('%', $1, $3); }
| '-' EXP %prec NEG        { $$ = creer_noeudOp('*',$2,creer_feuilleNb(-1));}
| ID                       { $$ = creer_feuilleId($1);}
;

%%

int main( int argc, char * argv[] ) {

  extern FILE *yyin;
  if (argc == 1){
    fprintf(stderr, "aucun fichier fourni\n");
    return 1;
  }
  yyin = fopen(argv[1],"r");
  fichier_ram = fopen("code.ram", "w");
 
  if(fichier_ram==NULL){
    printf("Erreur lors de l'ouverture d'un fichier");
    exit(1);
  }
  yyparse();
  return 0;
}

