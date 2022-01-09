#ifndef LISTES_H
#define LISTES_H

#define  IS_SEQ '1'
#define  IS_INT '0'

/*
 * Pour réaliser des tests de performance, désactiver tous les
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

#include <stdbool.h>
#include <stdlib.h>

extern bool silent_mode;

typedef struct cellule {
	char command;
	int value;
	struct sequence *seq;
	struct cellule *suivant;
} cellule;

typedef struct sequence{
	cellule *tete;
	cellule *fin;
} sequence;

/* renvoie une sequence */
sequence *init_seq();

/* renvoie une cellule */
cellule *init_cellule();

/* crée une cellule contenant un charactère (renvoie son addresse) */
cellule* empiler_char(char c, sequence *seq);

/* crée une cellule contenant un entier (renvoie son addresse) */
cellule* empiler_int(int n, sequence*seq);

/* crée une cellule contenant un groupe (renvoie son addresse) */
cellule* empiler_groupe(sequence *seq);

/* crée une cellule contenant un groupe vide */
cellule* creer_groupe(cellule *next);

/* recupère un entier dans la sequence */
int depiler_int(sequence *seq);

/* affiche la séquence */
void print_seq(sequence *seq);

/* conversion recursive en liste enchaînée */
void conversion(char **texte, sequence *seq);

/* destruction recursive, libération de mémoire */
void detruireGroupe(sequence *seq);

/* dépile un groupe de commande */
void depile_routine(sequence *seq, sequence *cel);

/* passe à la cellule suivant */
void suivant(sequence *seq, cellule *prev);

/* lie une cellule au début d'une séquence */
void lier(sequence *seq, cellule *cell);

#endif
