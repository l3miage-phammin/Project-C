#include "calculette.h"
#include "listes.h"

#define OPERATION(P, A, B, OPERATION)	\
					\
	int a, b;			\
					\
	A = depiler_int(P);		\
	B = depiler_int(P);		\
	empiler_int(B OPERATION A, P);

void
addition(sequence *p)
{
	OPERATION(p, a, b, +);
}

void
multiplication(sequence *p)
{
	OPERATION(p, a, b, *);
}

void
soustraction(sequence *p)
{
	OPERATION(p, a, b, -);
}

void
division(sequence *p)
{
	OPERATION(p, a, b, /);
}
