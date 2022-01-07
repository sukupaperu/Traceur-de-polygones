#include "Polygone.h"
#include "ListeTriee.h"
#include <stdlib.h>
#include <stdio.h>

Polygone* newPoly() {
	Polygone* p = (Polygone*)malloc(sizeof(Polygone));

	p->sommetDebut = NULL;
	p->sommetFin = NULL;
	p->sommetSelectionne = NULL;
	p->nbCotes = 0;
	p->couleurRemplissage = newCouleur(.09, .29, .68);
	p->couleurTracage = newCouleur(1, 1, 1);
	p->estRempli = p->estFerme = 0;
	p->estTrace = 1;

	return p;
}

void freePoly(Polygone* p) {
	Sommet* s = p->sommetDebut;
	while(s != NULL) {
		Sommet* sSuivantTemp = s->suivant;
		free(s);
		s = sSuivantTemp;
	}
	free(p);
}

void setEstRempliPoly(Polygone* p, int b) {	p->estRempli = b; }
int getEstRempliPoly(Polygone* p) {	return p->estRempli; }
void setEstFermePoly(Polygone* p, int b) { p->estFerme = b; }
int getEstFermePoly(Polygone* p) { return p->estFerme; }

Sommet* getSecondSommetAretePoly(Polygone* p, Sommet* s) {
	return s->suivant == NULL ? p->sommetDebut : s->suivant;
}
Sommet* getSecondSommetAreteSelectionnePoly(Polygone* p) {
	return getSecondSommetAretePoly(p, p->sommetSelectionne);
}

void ajouterSommetPoly(Polygone* p, Point pt) {
	Sommet* newS = (Sommet*)malloc(sizeof(Sommet));

	newS->position = pt;

	if(p->sommetFin == NULL)
		p->sommetFin = newS;

	newS->suivant = p->sommetDebut;
	if(p->sommetDebut != NULL)
		p->sommetDebut->precedent = newS;
	newS->precedent = p->sommetFin;
	p->sommetDebut = newS;

	p->sommetSelectionne = newS;
	p->nbCotes++;
}

Point pointMilieuPoly(Point a, Point b) {
	return newPoint((a.x + b.x)/2, (a.y + b.y)/2);
}

void diviserAreteSelectionnePoly(Polygone* p) {
	if(p->sommetSelectionne == NULL || p->nbCotes < 3)
		return;

	Sommet* newS = (Sommet*)malloc(sizeof(Sommet));

	Sommet* sA = p->sommetSelectionne;
	Sommet* sB = getSecondSommetAreteSelectionnePoly(p);

	newS->position = pointMilieuPoly(sA->position, sB->position);

	if(sA == p->sommetFin) {
		p->sommetFin = newS;
		newS->suivant = NULL;
	} else
		newS->suivant = sB;
	newS->precedent = sA;
	sA->suivant = newS;
	sB->precedent = newS;

	p->sommetSelectionne = newS;
	p->nbCotes++;
}

void retirerSommetSelectionnePoly(Polygone* p) {
	if(p->sommetSelectionne == NULL || p->sommetDebut == NULL)
		return;

	Sommet* s = p->sommetSelectionne;

	if(s == p->sommetDebut) {
		if(s == p->sommetFin) {
			p->sommetDebut = NULL;
			p->sommetFin = NULL;
			p->sommetSelectionne = NULL;
		} else {
			if(s->suivant != NULL)
				s->suivant->precedent = s->precedent;
			p->sommetDebut = s->suivant;
			p->sommetSelectionne = s->suivant;
		}
	} else if(s == p->sommetFin) {
		s->precedent->suivant = NULL;
		p->sommetFin = s->precedent;
		p->sommetDebut->precedent = p->sommetFin;
		p->sommetSelectionne = p->sommetDebut;
	} else {
		s->precedent->suivant = s->suivant;
		s->suivant->precedent = s->precedent;
		p->sommetSelectionne = s->suivant;
	}

	free(s);

	p->nbCotes--;
}

void retirerAreteSelectionnePoly(Polygone* p) {
	if(p->nbCotes < 2)
		return;
	retirerSommetSelectionnePoly(p);
	retirerSommetSelectionnePoly(p);
	avancerSelectionSommetPoly(p);
}

void decalerSommetSelectionnePoly(Polygone* p, Point decalage) {
	if(p->sommetSelectionne == NULL)
		return;

	p->sommetSelectionne->position.x += decalage.x;
	p->sommetSelectionne->position.y += decalage.y;
}

void deplacerSommetSelectionnePoly(Polygone* p, Point nouvellePosition) {
	if(p->sommetSelectionne == NULL)
		return;

	p->sommetSelectionne->position.x = nouvellePosition.x;
	p->sommetSelectionne->position.y = nouvellePosition.y;
}

void avancerSelectionSommetPoly(Polygone* p) {
	if(p->sommetSelectionne == NULL)
		return;

	p->sommetSelectionne = p->sommetSelectionne->precedent;
}

void reculerSelectionSommetPoly(Polygone* p) {
	if(p->sommetSelectionne == NULL)
		return;

	p->sommetSelectionne = p->sommetSelectionne->suivant == NULL ?
		p->sommetDebut : p->sommetSelectionne->suivant;
}

Sommet* getPlusProcheSommetPoly(Polygone* p, Point pt) {
	Sommet* s = p->sommetDebut;
	if(s == NULL)
		return NULL;

	int distMin = approxDistancePoint(s->position, pt);
	Sommet* sommetMin = s;

	s = s->suivant;

	while(s != NULL) {

		int dist = approxDistancePoint(s->position, pt);
		if(dist < distMin) {
			distMin = dist;
			sommetMin = s;
		}

		s = s->suivant;
	}

	return sommetMin;
}

void selectionnerPlusProcheSommetPoly(Polygone* p, Point pt) {
	if(p->sommetDebut == NULL)
		return;

	Sommet* proche = getPlusProcheSommetPoly(p, pt);
	if(approxDistancePoint(proche->position, pt) < POLY_DISTANCE_SELECTION_MIN)
		p->sommetSelectionne = proche;
}

Sommet* getPlusProcheAretePoly(Polygone* p, Point pt) {
	Sommet* s = p->sommetDebut;
	if(s == NULL || p->nbCotes < 2)
		return NULL;

	int distMin = approxDistanceSegment(p->sommetFin->position, p->sommetDebut->position, pt);
	Sommet* sommetMin = p->sommetFin;

	while(s->suivant != NULL) {

		int dist = approxDistanceSegment(s->position, s->suivant->position, pt);
		if(dist < distMin) {
			distMin = dist;
			sommetMin = s;
		}

		s = s->suivant;
	}

	return sommetMin;
}

void selectionnerPlusProcheAretePoly(Polygone* p, Point pt) {
	if(p->sommetDebut == NULL || p->nbCotes < 2)
		return;

	Sommet* proche = getPlusProcheAretePoly(p, pt);
	
	//if(approxDistanceSegment(proche->position, getSecondSommetAretePoly(p, proche)->position, pt) < POLY_DISTANCE_SELECTION_MIN)
	p->sommetSelectionne = proche;
}

Point getPositionSommetSelection(Polygone* p) {
	if(p->sommetSelectionne == NULL)
		return newPoint(0, 0);

	return p->sommetSelectionne->position;
}

Point getPositionSommetsAreteSelection(Polygone* p, Point* pt) {
	if(p->sommetSelectionne == NULL || p->nbCotes < 2)
		return *pt = newPoint(0, 0);

	*pt = getSecondSommetAreteSelectionnePoly(p)->position;

	return p->sommetSelectionne->position;
}

void getYminMaxPoly(Polygone* p, int* yMin, int* yMax) {
	Sommet* s = p->sommetDebut;
	*yMax = 0;

	if(s != NULL) {

		*yMin = s->position.y;
		if(s->position.y > *yMax)
			*yMax = s->position.y;

		s = s->suivant;
		while(s != NULL) {

			Point P = s->position;
			if(P.y < *yMin)
				*yMin = P.y;
			if(P.y > *yMax)
				*yMax = P.y;

			s = s->suivant;
		}
	} else
		*yMin = *yMax;
}

// tester s'il y a intersection entre l'horizontale de position yScan
// et le segment AB
int intersectionPoly(int yScan, Point A, Point B) {
	if(
		// on vérifie que la ligne horizontale yScan peut intersecter avec le segment AB
		// 'valeurEstEncadree' est définie dans le fichier 'Point.c'
		valeurEstEncadree(A.y, yScan, B.y)
		// on vérifie que le segment AB n'est pas un segment horizontal
		// (auquel cas on ne pourrait pas déduire une seule et unique intersection)
		&& A.y != B.y
	) {

		// si le segment AB est vertical, alors le calcul d'intersection est totalement trivial
		// C'est un cas rare, mais indispensable à traiter car le calcul ci-dessous ne fonctionne pas dans ce cas précis (division par 0)
		if(A.x == B.x)
			return A.x;

		// sinon, calcul de la position xScan de l'intersection
		float a = (float)(A.y - B.y)/(float)(A.x - B.x); // calcul de la pente de la droite affine passant par A et B
		return (float)((float)yScan - ((float)A.y - a*(float)A.x))/a; // calcul de xScan
	}
	// on considère qu'il n'y a pas d'intersection
	return -1;
}

// Permet de tracer le contour du polygone
void tracerPoly(Polygone* p, ZoneDessin* z, Couleur c, int estFerme) {
	if(p->nbCotes < 2)
		return;

	setCouleurZD(z, c);

	Sommet* s = p->sommetDebut;
	if(s == NULL)
		return;
	while(s->suivant != NULL) {
		bresenhamZD(z, s->position, s->suivant->position);

		s = s->suivant;
	}
	if(estFerme)
		bresenhamZD(z, s->position, p->sommetDebut->position);
}

// Permet de remplir le polygone p, dans la zone de dessin z, avec la couleur c
void remplirPoly(Polygone* p, ZoneDessin* z, Couleur c) {
	// si le polygone a moins de 3 côtés, on ne peut pas le remplir
	if(p->nbCotes < 3)
		return;

	// on définit la couleur de remplissage
	setCouleurZD(z, c);

	// on définit la position en y minimale et maximale des sommets du polygone
	int yMin, yMax;
	getYminMaxPoly(p, &yMin, &yMax);

	// liste des positions x des intersections trouvées
	ListeTriee* liste = newListeTriee();
	// on parcourt toute la zone de remplissage ligne par ligne
	for(int yScan = yMax; yScan >= yMin; yScan--) {

		// on parcourt la liste de sommets du polygone
		Sommet* s = p->sommetDebut;
		while(s->suivant != NULL) {

			// on cherche une intersection entre la ligne de scan courante
			// et une arrête (formée de deux sommets du polygone)
			int inter = intersectionPoly(yScan, s->position, s->suivant->position);
			if(inter >= 0) ajouterListeTriee(liste, inter);

			s = s->suivant;
		}
		// il ne faut pas oublier de tester l'arête formée par le premier et le dernier sommet
		int inter = intersectionPoly(yScan, s->position, p->sommetDebut->position);
		if(inter >= 0) ajouterListeTriee(liste, inter);

		// maintenant que l'on connait toutes les intersections sur la ligne de scan courante
		// on peut remplir
		while(!estVideListeTriee(liste)) {
			// on retire les intersections deux à deux
			int a = retirerListeTriee(liste), b = retirerListeTriee(liste);
			// if(b != -1)
				// j'aurais pu utiliser une fonction plus triviale pour dessiner
				// un simple segment horizontal
				bresenhamZD(z, newPoint(a, yScan), newPoint(b, yScan));
		}

	}
	// libération de la liste triée en mémoire
	freeListeTriee(liste);
}

void dessinerSommetsPoly(Polygone* p, ZoneDessin* z) {
	Sommet* s = p->sommetDebut;

	if(s == NULL)
		return;

	setCouleurZD(z, newCouleur(1, 1, 1));
	
	while(s != NULL) {
		remplirCarreZD(z, s->position, 2);
		s = s->suivant;
	}
}

void dessinerPoly(Polygone* p, ZoneDessin* z) {
	if(p->estRempli)
		remplirPoly(p, z, p->couleurRemplissage);
	if(p->estTrace)
		tracerPoly(p, z, p->couleurTracage, p->estFerme);
	dessinerSommetsPoly(p, z);
}

void dessinerSelectionSommetPoly(Polygone* p, ZoneDessin* z) {
	if(p->sommetSelectionne == NULL)
		return;

	setCouleurZD(z, newCouleur(1, 0, 0));

	tracerCarreZD(z, p->sommetSelectionne->position, 5);
}

void dessinerSelectionAretePoly(Polygone* p, ZoneDessin* z) {
	if(p->sommetSelectionne == NULL || p->nbCotes < 2)
		return;

	setCouleurZD(z, newCouleur(1, 0, 0));

	Point positionSelection = p->sommetSelectionne->position;
	Point positionSuivante = getSecondSommetAreteSelectionnePoly(p)->position;

	bresenhamZD(
		z,
		positionSuivante,
		positionSelection
	);

	tracerCarreZD(z, positionSuivante, 3);
	tracerCarreZD(z, positionSelection, 3);

}

void dessinerProchaineAreteAjouteePoly(Polygone* p, ZoneDessin* z, Point pt) {
	if(p->sommetDebut == NULL)
		return;

	setCouleurZD(z, newCouleur(1, 0, 0));

	bresenhamZD(z, p->sommetDebut->position, pt);

	if(p->estFerme)
		bresenhamZD(z, p->sommetFin->position, pt);

	remplirCarreZD(z, pt, 2);
}