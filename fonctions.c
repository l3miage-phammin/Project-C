#include "fonctions.h"
#include "listes.h"

void
echange(sequence *pile)
{
	cellule *tmp;

	tmp = pile->tete->suivant;
	pile->tete->suivant = pile->tete->suivant->suivant;
	tmp->suivant = pile->tete;
	pile->tete = tmp;
}

void
copy_groupe(sequence *source, sequence *dest)
{
	cellule *slice = source->tete, *new;

	while(slice != NULL) {
		switch(slice->command) {
		case(IS_INT):
			new = empiler_int(slice->value, NULL);
			lier(dest, new);
			break;
		case(IS_SEQ):
			new = creer_groupe(NULL);
			new->seq = init_seq();
			lier(dest, new);
			copy_groupe(slice->seq, new->seq);
			break;
		default:
			new = empiler_char(slice->command, NULL);
			lier(dest, new);
		}
		slice = slice->suivant;
	}
}

void
rotation(sequence *p, int dec, int len)
{
	if (len==dec || dec==0)
		return;

	int nb;

	if ((nb = len - (dec % len)) < 1)
		nb = 0;


	cellule *head, *tail, *before_head, *after_tail, *temp = p->tete;
	int i;

	for (i = 1; i < len+1; i++){
		if (i == nb )
			before_head = temp;
		else if (i == nb+1)
			head = temp;
		else if (i==len)
			tail = temp;
		temp = temp->suivant;
	}
	after_tail = temp;

	if (dec == 1) tail = head;

	before_head->suivant = after_tail;
	tail->suivant = p->tete;
	p->tete = head;
}
