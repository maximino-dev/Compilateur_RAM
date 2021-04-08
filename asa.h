/* 
  BOGADO GARCIA Maximino
  L3 Informatique

    asa.h
*/
#ifndef ASA_H
#define ASA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ts.h"

typedef enum {typeNb, typeOp, typeId, typeDecl, typeAffect, typeInst, typeTq, typeComp, typeBool, typeNon, typeSi, typeAfficher, typeLire} typeNoeud;

typedef struct {
  int val;
} feuilleNb;

typedef struct {
  char nom[64];
} feuilleId;

typedef struct {
  int ope;
  struct asa * noeud[2];
} noeudOp;

typedef struct {
  // Un buffer de 3 caracteres pour pouvoir stocker par exemple "!=" et le '\0'
  char ope[3];
  struct asa * noeud[2];
} noeudComp;

typedef struct {
  // Un buffer de 3 caracteres pour pouvoir stocker par exemple "OU" et le '\0'
  char ope[3];
  struct asa * noeud[2];
} noeudBool;

typedef struct {
  struct asa * noeud[2];
} noeudAffect;

typedef struct {
  struct asa * noeud;
} noeudAfficher;

typedef struct {
  struct asa * noeud;
} noeudLire;

typedef struct {
  struct asa * noeud[2];
} noeudTq;

typedef struct {
  struct asa * noeud[3];
} noeudSi;

typedef struct {
  struct asa * noeud;
  int size;
} noeudDecl;

typedef struct {
	struct asa *noeud[2];
} noeudInst;

typedef struct {
  struct asa * noeud;
} noeudNon;

typedef struct asa{
  typeNoeud type;
  int ninst;
 
  union {
    feuilleNb nb;
    feuilleId id;
    noeudInst inst;
    noeudDecl decl;
    noeudAffect affect;
    noeudComp comp;
    noeudOp op;
    noeudTq tq;
    noeudBool booleen;
    noeudNon non;
    noeudSi si;
    noeudAfficher afficher;
    noeudLire lire;
  };
} asa;

// fonction d'erreur utilisée également par Bison
void yyerror(const char * s);

/*
  Les fonctions creer_<type> construise un noeud de l'arbre
  abstrait du type correspondant et renvoie un pointeur celui-ci
 */
asa * creer_feuilleNb( int value );
asa * creer_noeudOp( int ope, asa * p1, asa * p2 );
asa * creer_feuilleId(char *s);
asa * creer_noeudAffect(asa * p1, asa * p2);

// On prend un char* car il peut y avoir "!=" ">=" et "<="
asa * creer_noeudComp(char *ope, asa * p1, asa * p2);
asa * creer_noeudSi(asa * p1, asa * p2, asa * p3);
asa * creer_noeudBool(char *ope, asa * p1, asa * p2);
asa * creer_noeudNon(asa * p1);
asa * creer_noeudAfficher(asa * p1);
asa * creer_noeudLire(asa * p1);
asa * creer_noeudDecl(asa * p1, int size);
asa * creer_noeudInst(asa * p1, asa * p2);
asa * creer_noeudTq(asa * p1, asa * p2);

void free_asa(asa *p);

// produit du code pour la machine RAM à partir de l'arbre abstrait
// ET de la table de symbole
void codegen(asa *p);

extern ts * tsymb;

#endif
