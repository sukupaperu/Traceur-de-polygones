#include "ZoneDessin.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

ZoneDessin* newZD(int l, int h) {
	ZoneDessin* z = (ZoneDessin*)malloc(sizeof(ZoneDessin));

	z->largeur = l;
	z->hauteur = h;
	z->couleur = newCouleur(1, 1, 1);

	z->grille = (Couleur**)malloc(sizeof(Couleur*)*l);

	for(int x = 0; x < l; x++) {
		z->grille[x] = (Couleur*)malloc(sizeof(Couleur)*h);
		for(int y = 0; y < z->hauteur; y++)
			z->grille[x][y] = z->couleur;
	}

	z->couleur = newCouleur(0, 0, 0);

	return z;
}

void freeZD(ZoneDessin* z) {

	for(int x = 0; x < z->largeur; x++)
		free(z->grille[x]);

	free(z->grille);
	free(z);
}

void remplirZD(ZoneDessin* z, Couleur c) {
	for(int x = 0; x < z->largeur; x++)
		for(int y = 0; y < z->hauteur; y++)
			z->grille[x][y] = c;
}

void setCouleurZD(ZoneDessin* z, Couleur c) {
	z->couleur = c;
}

void pointZD(ZoneDessin* z, Point p) {
	if(p.x >= 0 && p.y >= 0 && p.x < z->largeur && p.y < z->hauteur)
		z->grille[p.x][p.y] = z->couleur;
}

void coordToPremierOctant(Point a, Point b, Point* aPO, Point* bPO) {
	Point aPQ, bPQ;

	if(a.x > b.x) aPQ.x = -a.x, bPQ.x = -b.x;
	else aPQ.x = a.x, bPQ.x = b.x;

	if(a.y > b.y) aPQ.y = -a.y, bPQ.y = -b.y;
	else aPQ.y = a.y, bPQ.y = b.y;

	if(abs(b.x - a.x) < abs(b.y - a.y))
		*aPO = newPoint(aPQ.y, aPQ.x), *bPO = newPoint(bPQ.y, bPQ.x);
	else
		*aPO = aPQ, *bPO = bPQ;
}

Point premierOctantToCoord(Point a, Point b, Point PO) {
	Point PQ;

	if(abs(b.x - a.x) < abs(b.y - a.y))
		PQ = newPoint(PO.y, PO.x);
	else PQ = PO;

	return newPoint(
		a.x > b.x ? -PQ.x : PQ.x,
		a.y > b.y ? -PQ.y : PQ.y
	);
}

void bresenhamZD(ZoneDessin* z, Point a, Point b) {
	Point APO, BPO;
	coordToPremierOctant(a, b, &APO, &BPO);
	int dx = BPO.x - APO.x, dy = BPO.y - APO.y;
	int incrd1 = 2*dy, incrd2 = 2*(dy - dx);
	int d = 2*dy - dx;
	while(APO.x < BPO.x) {
		pointZD(z, premierOctantToCoord(a, b, APO));
		APO.x++;
		if(d < 0)
			d += incrd1;
		else {
			APO.y++;
			d += incrd2;
		}
	}
	pointZD(z, premierOctantToCoord(a, b, APO));
}

void tracerCarreZD(ZoneDessin* z, Point p, int taille) {
	Point a = newPoint(p.x - taille, p.y - taille),
		b = newPoint(p.x - taille, p.y + taille),
		c = newPoint(p.x + taille, p.y + taille),
		d = newPoint(p.x + taille, p.y - taille);

	bresenhamZD(z, a, b);
	bresenhamZD(z, b, c);
	bresenhamZD(z, c, d);
	bresenhamZD(z, d, a);
}

void remplirCarreZD(ZoneDessin* z, Point p, int taille) {
	for(int x = p.x - taille; x <= p.x + taille; x++)
		for(int y = p.y - taille; y <= p.y + taille; y++)
			pointZD(z, newPoint(x, y));
}

void dessinerTexteZD(Point pos, char* texte, Couleur co) {
	glColor3ub(co.r*255, co.g*255, co.b*255);
	glRasterPos2i(pos.x, pos.y);
	char c;
	int i = 0;
	while((c = texte[i++]) != '\0')
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

void dessinerZD(ZoneDessin* z) {
	glBegin(GL_POINTS);
	for(int x = 0; x < z->largeur; x++)
		for(int y = 0; y < z->hauteur; y++) {
				Couleur c = z->grille[x][y];
				glColor3ub(255*c.r, 255*c.g, 255*c.b);
				glVertex2i(x, y);
			}
	glEnd();
}