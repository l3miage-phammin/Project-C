#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "calculette.h"
#include "curiosity.h"
#include "fonctions.h"

void stop (void)
{
	char enter = '\0';
	printf ("Appuyer sur entrée pour continuer...\n");
	while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

int interprete (sequence* seq, bool debug)
{
	char commande;
	int ret;
	sequence *pile = init_seq(), *tmp;
	cellule *prev = NULL;

	debug = false;

	printf ("Programme:");
	print_seq(seq);
	printf ("\n");
	if (debug) stop();


	while (seq->tete != NULL) {
		commande = seq->tete->command;

		switch (commande) {

		case 'A':
			ret = avance();
			if (ret == VICTOIRE) {
				detruireGroupe(seq);
				detruireGroupe(pile);
				free(pile);
				return VICTOIRE;
			}
			if (ret == RATE) {
				detruireGroupe(seq);
				detruireGroupe(pile);
				free(pile);
				return RATE;
			}
			break;

		case 'G':
			gauche();
			break;

		case 'D':
			droite();
			break;

		case '+':
			addition(pile);
			break;

		case '-':
			soustraction(pile);
			break;

		case '*':
			multiplication(pile);
			break;

		case '/':
			division(pile);
			break;

		case 'P':
			pose(depiler_int(pile));
			break;

		case 'M':
			empiler_int(mesure(depiler_int(pile)), pile);
			break;

		case 'X':
			echange(pile);
			break;

		case 'I':
			if (pile->tete->command == IS_SEQ)
				detruireGroupe(pile->tete->seq);
			suivant(pile, prev);
			break;

		case '!':
			suivant(seq, prev);
			depile_routine(seq, pile->tete->seq);
			suivant(pile, prev);
			goto affichage;

		case IS_INT:
			empiler_int(seq->tete->value, pile);
			break;

		case IS_SEQ:
			prev = seq->tete->suivant;
			seq->tete->suivant = pile->tete;
			pile->tete = seq->tete;
			seq->tete = prev;
			goto affichage;

		case '?':
			suivant(seq, prev);
			if (pile->tete->suivant->suivant->value == 0) {
				depile_routine(seq, pile->tete->seq);
				suivant(pile, prev);
				detruireGroupe(pile->tete->seq);
				suivant(pile, prev);
			} else {
				detruireGroupe(pile->tete->seq);
				suivant(pile, prev);
				depile_routine(seq, pile->tete->seq);
				suivant(pile, prev);
			}
			depiler_int(pile);
			goto affichage;

		case 'C':
			suivant(seq, prev);

			switch(pile->tete->command) {
			case(IS_SEQ):
				pile->tete = creer_groupe(pile->tete);
				pile->tete->seq = init_seq();
				copy_groupe(pile->tete->suivant->seq, pile->tete->seq);
				break;
			case(IS_INT):
				prev = empiler_int(pile->tete->value, pile);
				pile->tete = prev;
				break;
			default:
				prev = empiler_char(pile->tete->command, pile);
				pile->tete = prev;
			}
			goto affichage;

		case 'B':
			if ((ret = depiler_int(pile)) == 0) {
				suivant(seq, prev);
				seq->tete = empiler_char('I', seq);
			} else {
				tmp = init_seq();
				copy_groupe(pile->tete->seq, tmp);
				tmp->fin->suivant = seq->tete;
				seq->tete = tmp->tete;
				pile->tete = empiler_int(ret - 1, pile);
				free(tmp);
			}
			goto affichage;

		case 'R':
			rotation(pile, depiler_int(pile), depiler_int(pile));
			break;

		default:
			break;
		}

		suivant(seq, prev);

affichage:
	afficherCarte();
	printf ("Programme:");
	print_seq(seq);
	printf ("\nPile: ");
	print_seq(pile);
	printf ("\n");


		if (debug)
			stop();
	}

	return CIBLERATEE;
}
