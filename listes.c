#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"

bool silent_mode = false;

sequence*
init_seq()
{
	sequence *seq = (sequence *) malloc(sizeof(sequence));

	seq->tete = NULL;
	seq->fin = NULL;

	return seq;
}

cellule*
init_cellule()
{
	cellule *new = (cellule*) malloc(sizeof(cellule));
	new->suivant = NULL;

	return new;
}

cellule*
empiler_char(char c, sequence *seq)
{
	cellule *new = init_cellule();

	if (seq != NULL) {
		new->suivant = seq->tete;
		seq->tete = new;
	}
	new->seq = NULL;
	new->command = c;

	return new;
}

cellule*
empiler_int(int n, sequence*seq)
{
	cellule *new = init_cellule();

	if (seq != NULL) {
		new->suivant = seq->tete;
		seq->tete = new;
	}
	new->command = IS_INT;
	new->seq = NULL;
	new->value = n;

	return new;
}

cellule*
creer_groupe(cellule *next)
{
	cellule *new = init_cellule();

	new->suivant = next;
	new->command = IS_SEQ;
	new->seq = NULL;

	return new;
}


void
depile_routine(sequence *seq, sequence *cel)
{
	if (cel->tete == NULL)
		return;

	cel->fin->suivant = seq->tete;
	seq->tete = cel->tete;
	cel->tete = NULL;
}

int
depiler_int(sequence*seq)
{
	int save = seq->tete->value;

	cellule *tmp = seq->tete;
	seq->tete=seq->tete->suivant;
	free(tmp);

	return save;
}

void
print_seq(sequence *seq)
{
	cellule *tmp = seq->tete;

	while(tmp != NULL) {
		switch(tmp->command) {
		case IS_SEQ:
			printf("{ ");
			print_seq(tmp->seq);
			printf("} ");
			break;
		case IS_INT:
			printf("%d ", tmp->value);
			break;
		default:
			printf("%c ", tmp->command);
			break;
		}
		tmp = tmp->suivant;
	}
}

void
detruireGroupe(sequence *seq)
{
	cellule *slice = seq->tete, *tmp = NULL;

	seq->tete = NULL;
	seq->fin = NULL;

	while (slice != NULL) {
		if (slice->command == IS_SEQ)
			detruireGroupe(slice->seq);
		free(slice->seq);
		tmp = slice;
		slice = slice->suivant;
		free(tmp);
	}
}

void
conversion(char **texte, sequence *seq)
{
	cellule *new;
	char c;

	while ((c = **texte) != '\0') {
		if ('0' <= c && c <= '9') {
			new = empiler_int( c - '0', NULL);
			lier(seq, new);
		} else if (c == '{') {
			new = creer_groupe(NULL);
			new->seq = init_seq();
			lier(seq, new);
			(*texte)++;
			conversion(texte, new->seq);
		} else if (c == '}') {
			return;
		} else {
			new = empiler_char(c, NULL);
			lier(seq, new);
		}
		(*texte)++;
	}
}

void suivant(sequence *seq, cellule *prev)
{
	prev = seq->tete;

	if (prev->command == IS_SEQ)
		free(prev->seq);

	seq->tete = seq->tete->suivant;
	free(prev);
}

void lier(sequence *seq, cellule *cell)
{
	if (seq->tete == NULL)
		seq->fin = seq->tete = cell;
	else
		seq->fin = seq->fin->suivant = cell;
}
