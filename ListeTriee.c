#include "ListeTriee.h"
#include <stdlib.h>

ListeTriee* newListeTriee() {
	ListeTriee* l = (ListeTriee*)malloc(sizeof(ListeTriee));

	l->tete = NULL;

	return l;
}

void freeListeTriee(ListeTriee* l) {
	ElementListeTriee* e = l->tete;
	while(e != NULL) {
		ElementListeTriee* eTemp = e->suivant;
		free(e);
		e = eTemp;
	}
	free(l);
}

int estVideListeTriee(ListeTriee* l) {
	return l->tete == NULL;
}

void ajouterListeTriee(ListeTriee* l, int v) {
	ElementListeTriee* newEl = (ElementListeTriee*)malloc(sizeof(ElementListeTriee));
	newEl->valeur = v;
	newEl->suivant = NULL;

	if(estVideListeTriee(l)) {
		l->tete = newEl;
	} else if(v > l->tete->valeur) {
		newEl->suivant = l->tete;
		l->tete = newEl;
	} else {
		ElementListeTriee* e = l->tete;
		while(e->suivant != NULL && v < e->suivant->valeur)
			e = e->suivant;
		if(e->suivant != NULL)
			newEl->suivant = e->suivant;
		e->suivant = newEl;
	}
}

int retirerListeTriee(ListeTriee* l) {
	if(estVideListeTriee(l))
		return -1;
	ElementListeTriee* eTemp = l->tete;
	int v = l->tete->valeur;
	l->tete = l->tete->suivant;
	free(eTemp);
	return v;
}

void parcourirListeTriee(ListeTriee* l, void (*actionSurElementListeTriee)(int)) {
	ElementListeTriee* e = l->tete;
	while(e != NULL) {
		actionSurElementListeTriee(e->valeur);
		e = e->suivant;
	}
}