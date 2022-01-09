#ifndef FONCTIONS_H
#define FONCTIONS_H

#include "listes.h"
#include "calculette.h"

cellule *cel_index(sequence*pile,int n);

void echange(sequence *pile);

void copy_groupe(sequence *source, sequence *dest);

void rotation(sequence *p, int dec, int len);

#endif /* FONCTIONS_H */
