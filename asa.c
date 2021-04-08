/* 
  BOGADO GARCIA Maximino
  L3 Informatique

    asa.c
*/
#include "asa.h"

static int pile = 1;
static int ligneno = 0;

FILE* fichier_ram;

asa * creer_feuilleNb(int val)
{
  asa *p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeNb;
  p->nb.val = val;
  p->ninst = 1;
  return p;
}

asa * creer_noeudInst(asa * p1, asa * p2)
{
  asa *p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");
  p->type = typeInst;
  p->inst.noeud[0] = p1;
  p->inst.noeud[1] = p2;
  p->ninst = p1->ninst + p2->ninst;
  return p;
}

asa * creer_feuilleId(char *s)
{
	asa *p;
	if ((p = malloc(sizeof(asa))) == NULL)
		yyerror("echec allocation mémoire");
		
	p->type = typeId;
  p->ninst = 1;
	strcpy(p->id.nom,s);
	return p;
}

asa * creer_noeudOp( int ope, asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeOp;
  p->op.ope = ope;
  p->op.noeud[0]=p1;
  p->op.noeud[1]=p2;
  p->ninst = p1->ninst+p2->ninst+2;
  
  return p;
}

asa * creer_noeudAffect(asa * p1, asa * p2)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeAffect;
  p->affect.noeud[0]=p1;
  p->affect.noeud[1]=p2;
  p->ninst = p2->ninst + 1;
  
  return p;
}

asa * creer_noeudComp(char *ope,asa * p1, asa * p2)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeComp;
  strcpy(p->comp.ope,ope);
  p->comp.noeud[0]=p1;
  p->comp.noeud[1]=p2;
  p->ninst = p1->ninst + p2->ninst + 6;
  
  return p;
}

asa * creer_noeudBool(char *ope, asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeBool;
  strcpy(p->booleen.ope,ope);
  p->booleen.noeud[0]=p1;
  p->booleen.noeud[1]=p2;
  if(strcmp("ET",ope) == 0)
  {
    p->ninst = p1->ninst + p2->ninst + 11;
  }
  else
  {
    p->ninst = p1->ninst + p2->ninst + 9;
  }
  
  return p;
}

asa * creer_noeudNon(asa * p1)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeNon;
  p->non.noeud = p1;
  p->ninst = p1->ninst + 4;
  
  return p;
}

asa * creer_noeudSi(asa * p1, asa * p2, asa * p3)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeSi;
  p->si.noeud[0] = p1;
  p->si.noeud[1] = p2;
  p->si.noeud[2] = p3;
  p->ninst = p1->ninst + p2->ninst + p3->ninst + 2;
  
  return p;
}

asa * creer_noeudTq(asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");
  p->type = typeTq;
  p->tq.noeud[0]=p1;
  p->tq.noeud[1]=p2;
  p->ninst = p1->ninst + p2->ninst + 2;

  return p;
}

asa * creer_noeudDecl(asa * p1, int size)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  if(ts_retrouver_id(p1->id.nom) == 0)
  {
  	ts_ajouter_id(p1->id.nom, size);
  }
  p->type = typeDecl;
  p->decl.noeud=p1;
  p->decl.size = size;
  p->ninst = 0;
  
  return p;
}

asa * creer_noeudAfficher(asa * p1)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeAfficher;
  p->afficher.noeud = p1;
  p->ninst = p1->ninst + 1;
  
  return p;
}

asa * creer_noeudLire(asa * p1)
{
  asa * p;
  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire");

  p->type = typeLire;
  p->lire.noeud = p1;
  p->ninst = 2;
  
  return p;
}


void free_asa(asa *p)
{
 
  if (!p){
    return;
  }
  switch (p->type) {
  case typeOp:
    free_asa(p->op.noeud[0]);
    free_asa(p->op.noeud[1]);
    break;
  case typeTq:
    free_asa(p->tq.noeud[0]);
    free_asa(p->tq.noeud[1]);
    break;
  case typeAffect:
    free_asa(p->affect.noeud[0]);
    free_asa(p->affect.noeud[1]);
    break;
  case typeInst:
    free_asa(p->inst.noeud[0]);
    free_asa(p->inst.noeud[1]);
    break;
  case typeDecl:
    free_asa(p->decl.noeud);
    break;
  case typeComp:
    free_asa(p->comp.noeud[1]);
    free_asa(p->comp.noeud[0]);
    break;
  case typeBool:
    free_asa(p->booleen.noeud[1]);
    free_asa(p->booleen.noeud[0]);
    break;
  case typeNon:
    free_asa(p->non.noeud);
    break;
  case typeSi:
    free_asa(p->si.noeud[0]);
    free_asa(p->si.noeud[1]);
    free_asa(p->si.noeud[2]);
    break;
  case typeLire:
    free_asa(p->lire.noeud);
    break;
  default: break;
  }
  free(p);
}


void codegen(asa *p)
{
  ts *pts;
  int ret;
  if (!p) return;
  switch(p->type) {
  case typeNb:
    fprintf(fichier_ram,"LOAD #%d\n", p->nb.val);
    ligneno++;
    break;
  case typeInst:
    codegen(p->inst.noeud[0]);
    codegen(p->inst.noeud[1]);
    break;
  case typeOp:
  	codegen(p->op.noeud[1]);
  	fprintf(fichier_ram,"STORE %d\n", pile++);
  	ligneno++;
  	codegen(p->op.noeud[0]);
  	switch(p->op.ope)
  	{
  		case '+':
  			fprintf(fichier_ram,"ADD %d\n", --pile);
  			ligneno++;
  			break;
  		case '-':
  			fprintf(fichier_ram,"SUB %d\n", --pile);
  			ligneno++;
  			break;
  		case '*':
  			fprintf(fichier_ram,"MUL %d\n", --pile);
  			ligneno++;
  			break;
  		case '/':
  			fprintf(fichier_ram,"DIV %d\n", --pile);
  			ligneno++;
  			break;
      case '%':
        fprintf(fichier_ram,"MOD %d\n", --pile);
        ligneno++;
        break;
  	}
    break;
  case typeComp:
    codegen(p->comp.noeud[1]);
    fprintf(fichier_ram,"STORE %d\n", pile++);
    ligneno++;
    codegen(p->comp.noeud[0]);
    if(strcmp(p->comp.ope,">")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUMG %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
    }
    else if(strcmp(p->comp.ope,"<")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUML %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
    }
    else if(strcmp(p->comp.ope,"=")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUMZ %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
    }
    else if(strcmp(p->comp.ope,"!=")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUMZ %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
    }
    else if(strcmp(p->comp.ope,">=")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUML %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
    }
    else if(strcmp(p->comp.ope,"<=")==0)
    {
      fprintf(fichier_ram,"SUB %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"JUMG %d\n", ligneno + 3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
    }
    break;
  case typeBool:
    codegen(p->booleen.noeud[1]);
    fprintf(fichier_ram,"STORE %d\n", pile++);
    ligneno++;
    codegen(p->booleen.noeud[0]);
    fprintf(fichier_ram,"SUB #1\n");
    ligneno++;
    if(strcmp(p->booleen.ope,"OU")==0)
    {
      fprintf(fichier_ram,"JUMZ %d\n", ligneno+6);
      ligneno++;
      fprintf(fichier_ram,"LOAD %d\n", pile--);
      ligneno++;
      fprintf(fichier_ram,"SUB #1\n");
      ligneno++;
      fprintf(fichier_ram,"JUMZ %d\n", ligneno+3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno + 2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
    }
    else if(strcmp(p->booleen.ope,"ET")==0)
    {
      fprintf(fichier_ram,"JUMZ %d\n", ligneno+3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno+7);
      ligneno++;
      fprintf(fichier_ram,"LOAD %d\n", --pile);
      ligneno++;
      fprintf(fichier_ram,"SUB #1\n");
      ligneno++;
      fprintf(fichier_ram,"JUMZ %d\n", ligneno+3);
      ligneno++;
      fprintf(fichier_ram,"LOAD #0\n");
      ligneno++;
      fprintf(fichier_ram,"JUMP %d\n", ligneno+2);
      ligneno++;
      fprintf(fichier_ram,"LOAD #1\n");
      ligneno++;
    }
    break;
  case typeNon:
    codegen(p->non.noeud);
    fprintf(fichier_ram,"JUMZ %d\n", ligneno+3);
    ligneno++;
    fprintf(fichier_ram,"LOAD #0\n");
    ligneno++;
    fprintf(fichier_ram,"JUMP %d\n", ligneno+2);
    ligneno++;
    fprintf(fichier_ram,"LOAD #1\n");
    ligneno++;
    break;
  case typeSi:
    codegen(p->si.noeud[0]);
    fprintf(fichier_ram,"JUMZ %d\n", ligneno + p->si.noeud[1]->ninst + 2);
    ligneno++;
    codegen(p->si.noeud[1]);
    fprintf(fichier_ram,"JUMP %d\n", ligneno + p->si.noeud[2]->ninst + 1);
    ligneno++;
    codegen(p->si.noeud[2]);
    break;
  case typeId:
  	if( (pts = ts_retrouver_id(p->id.nom)) == 0)
    {
      fprintf(stderr, "Variable %s introuvable\n", p->id.nom);
      exit(1);
    }
    fprintf(fichier_ram,"LOAD %d\n", pts->adr);
    ligneno++;
    break;
  case typeAffect:
    codegen(p->affect.noeud[1]);
    if( (pts = ts_retrouver_id( p->affect.noeud[0]->id.nom)) == 0)
    {
      fprintf(stderr, "variable %s introuvable\n", p->affect.noeud[0]->id.nom);
      exit(1);
    }
    fprintf(fichier_ram,"STORE %d\n", pts->adr);
    ligneno++;
    break;
  case typeTq:
    ret = ligneno;
    codegen(p->tq.noeud[0]);
    fprintf(fichier_ram,"JUMZ %d\n", ligneno + p->tq.noeud[1]->ninst + 2);
    ligneno++;
    codegen(p->tq.noeud[1]);
    fprintf(fichier_ram,"JUMP %d\n", ret);
    ligneno++;
    break;
  case typeAfficher:
    codegen(p->afficher.noeud);
    fprintf(fichier_ram,"WRITE\n");
    ligneno++;
    break;
  case typeLire:
    fprintf(fichier_ram,"READ\n");
    ligneno++;
    if( (pts = ts_retrouver_id( p->lire.noeud->id.nom)) == 0)
    {
      fprintf(stderr, "variable %s introuvable\n", p->affect.noeud[0]->id.nom);
      exit(1);
    }
    fprintf(fichier_ram,"STORE %d\n", pts->adr);
    ligneno++;
    break;
  default:
    break;
  }
}

void yyerror(const char * s)
{
  fprintf(stderr, "%s\n", s);
  exit(0);
}
