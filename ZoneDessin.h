#ifndef _ZONE_DESSIN_H_
#define _ZONE_DESSIN_H_

#include "Couleur.h"
#include "Point.h"

typedef struct {
	int largeur, hauteur;
	Couleur couleur;
	Couleur** grille;
} ZoneDessin;

ZoneDessin* newZD(int, int);
void freeZD(ZoneDessin*);
void remplirZD(ZoneDessin*, Couleur);
void setCouleurZD(ZoneDessin*, Couleur);
void pointZD(ZoneDessin*, Point);
void bresenhamZD(ZoneDessin*, Point, Point);
void tracerCarreZD(ZoneDessin*, Point, int);
void remplirCarreZD(ZoneDessin*, Point, int);
void dessinerTexteZD(Point, char*, Couleur);
void dessinerZD(ZoneDessin*);

#endif