#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include "Point.h"
#include "ZoneDessin.h"

#define POLY_DISTANCE_SELECTION_MIN 500

typedef struct Sommet_t {
	Point position;
	struct Sommet_t* suivant;
	struct Sommet_t* precedent;
} Sommet;

typedef struct {
	Sommet* sommetDebut;
	Sommet* sommetFin;
	Sommet* sommetSelectionne;
	int nbCotes;
	Couleur couleurRemplissage;
	Couleur couleurTracage;
	int estRempli;
	int estTrace;
	int estFerme;
} Polygone;

Polygone* newPoly();
void freePoly(Polygone*);

void setEstRempliPoly(Polygone*, int);
int getEstRempliPoly(Polygone*);
void setEstFermePoly(Polygone*, int);
int getEstFermePoly(Polygone*);

void ajouterSommetPoly(Polygone*, Point);
void diviserAreteSelectionnePoly(Polygone*);

void retirerSommetSelectionnePoly(Polygone*);
void retirerAreteSelectionnePoly(Polygone*);
void decalerSommetSelectionnePoly(Polygone*, Point);
void deplacerSommetSelectionnePoly(Polygone*, Point);

void avancerSelectionSommetPoly(Polygone*);
void reculerSelectionSommetPoly(Polygone*);

void selectionnerPlusProcheSommetPoly(Polygone*, Point);
void selectionnerPlusProcheAretePoly(Polygone*, Point);

Point getPositionSommetSelection(Polygone*);
Point getPositionSommetsAreteSelection(Polygone*, Point*);

void dessinerPoly(Polygone*, ZoneDessin*);
void tracerPoly(Polygone*, ZoneDessin*, Couleur, int);
void remplirPoly(Polygone*, ZoneDessin*, Couleur);
void dessinerSelectionSommetPoly(Polygone*, ZoneDessin*);
void dessinerSelectionAretePoly(Polygone*, ZoneDessin*);
void dessinerProchaineAreteAjouteePoly(Polygone*, ZoneDessin*, Point);

#endif