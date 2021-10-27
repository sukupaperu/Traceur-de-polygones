#ifndef _LISTETRIEE_H_
#define _LISTETRIEE_H_

typedef struct ElementListeTriee_t {
	int valeur;
	struct ElementListeTriee_t* suivant;
} ElementListeTriee;

typedef struct {
	ElementListeTriee* tete;
} ListeTriee;

ListeTriee* newListeTriee();
void freeListeTriee(ListeTriee*);
int estVideListeTriee(ListeTriee*);
void ajouterListeTriee(ListeTriee*, int);
int retirerListeTriee(ListeTriee*);
void parcourirListeTriee(ListeTriee*, void (*)(int));

#endif