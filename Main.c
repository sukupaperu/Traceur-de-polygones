#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ZoneDessin.h"
#include "Polygone.h"

typedef enum { MODE_APPEND, MODE_VERTEX, MODE_EDGE } Mode;

Mode modeCourant;
ZoneDessin* zDessin;
Polygone* poly;
Point positionSouris;
int dagEstActif, aideAffichees;

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	dessinerZD(zDessin);

	int pX = 4;
	if(aideAffichees) {
		dessinerTexteZD(newPoint(pX, 15*1), "(h) cacher aides", newCouleur(1, 1, 1));
		dessinerTexteZD(newPoint(pX, 15*3), "(echap) quitter", newCouleur(1, 1, 1));

		dessinerTexteZD(newPoint(pX, 15*5), "(i) mode append", newCouleur(1, 1, 1));
		dessinerTexteZD(newPoint(pX, 15*6), "(v) mode vertex", newCouleur(1, 1, 1));
		dessinerTexteZD(newPoint(pX, 15*7), "(e) mode edge", newCouleur(1, 1, 1));

		dessinerTexteZD(newPoint(pX, 15*9), "(f) remplir le polygone", newCouleur(1, 1, 1));
		dessinerTexteZD(newPoint(pX, 15*10), "(c) polygone ferme/ouvert", newCouleur(1, 1, 1));
		switch(modeCourant) {
			case MODE_APPEND:
				dessinerTexteZD(newPoint(pX, 15*12), "(clic) ajouter sommet", newCouleur(1, 1, 1));
				break;
			case MODE_VERTEX:
				dessinerTexteZD(newPoint(pX, 15*12), "(page up/down) selectionner sommet suivant/precedent", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*13), "(clic) selectionner/deplacer sommet", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*14), "(suppr) supprimer sommet selectionne", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*15), "(fleches) deplacer sommet selectionne", newCouleur(1, 1, 1));
				break;
			case MODE_EDGE:
				dessinerTexteZD(newPoint(pX, 15*12), "(page up/down) selectionner arete suivante/precedente", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*13), "(clic) selectionner arete", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*14), "(suppr) supprimer arete selectionnee", newCouleur(1, 1, 1));
				dessinerTexteZD(newPoint(pX, 15*15), "(r/clic molette) diviser arete selectionnee", newCouleur(1, 1, 1));
				break;
		}
	} else {
		dessinerTexteZD(newPoint(pX, 15*1), "(h) afficher aides", newCouleur(1, 1, 1));
	}


	Point pTexte = newPoint(zDessin->largeur - 4, zDessin->hauteur - 13/2);
	char t[30];
	switch(modeCourant) {
		case MODE_APPEND:
			pTexte.x -= 11*8;
			dessinerTexteZD(pTexte, "Mode append", newCouleur(1, 1, 1));
			break;

		case MODE_VERTEX:
			pTexte.x -= 11*8;
			dessinerTexteZD(pTexte, "Mode vertex", newCouleur(1, 1, 1));

			Point p = getPositionSommetSelection(poly);
			sprintf(t, "(%i, %i)", p.x, p.y);
			dessinerTexteZD(newPoint(4, pTexte.y), t, newCouleur(1, 1, 1));
			break;

		case MODE_EDGE:
			pTexte.x -= 9*8;
			dessinerTexteZD(pTexte, "Mode edge", newCouleur(1, 1, 1));

			Point p2, p1 = getPositionSommetsAreteSelection(poly, &p2);
			sprintf(t, "(%i, %i) (%i, %i)", p1.x, p1.y, p2.x, p2.y);
			dessinerTexteZD(newPoint(4, pTexte.y), t, newCouleur(1, 1, 1));
			break;
	}

	glutSwapBuffers();
}

void majDessin() {
	remplirZD(zDessin, newCouleur(.04, .22, .17));

	dessinerPoly(poly, zDessin);
	switch(modeCourant) {
		case MODE_APPEND:
			dessinerProchaineAreteAjouteePoly(poly, zDessin, positionSouris);
			break;
		case MODE_VERTEX:
			dessinerSelectionSommetPoly(poly, zDessin);
			break;
		case MODE_EDGE:
			dessinerSelectionAretePoly(poly, zDessin);
			break;
		default: break;
	}
	
	glutPostRedisplay();
}

void keyEvent(unsigned char key, int x, int y) {

	// printf("%i %i\n", key, 'H');

	switch(key) {
		case 'h': aideAffichees = !aideAffichees; break;
		case 'i': modeCourant = MODE_APPEND; break;
		case 'v': modeCourant = MODE_VERTEX; break;
		case 'e': modeCourant = MODE_EDGE; break;
		case 'c': setEstFermePoly(poly, !getEstFermePoly(poly)); break;
		case 'f': setEstRempliPoly(poly, !getEstRempliPoly(poly)); break;

		case 127: // suppr
			switch(modeCourant) {
				case MODE_VERTEX: retirerSommetSelectionnePoly(poly); break;
				case MODE_EDGE:	retirerAreteSelectionnePoly(poly); break;				
				default: return;
			}
			break;

		case 'r':
			if(modeCourant != MODE_EDGE) return;
			diviserAreteSelectionnePoly(poly);
			break;

		case 27: // echap
			freeZD(zDessin);
			freePoly(poly);
			printf("Fin du programme.\n");
			exit(0);
			break;

		default: return;
	}

	majDessin();

}

void specialEvent(int key, int x, int y) {

	switch(key)	{
	 	case GLUT_KEY_LEFT:
	 	case GLUT_KEY_UP:
		case GLUT_KEY_RIGHT:
	 	case GLUT_KEY_DOWN:
	 		if(modeCourant != MODE_VERTEX) return;
	 		decalerSommetSelectionnePoly(poly, newPoint(
	 			key == GLUT_KEY_LEFT ? -1 : key == GLUT_KEY_RIGHT ? 1 : 0,
	 			key == GLUT_KEY_UP ? -1 : key == GLUT_KEY_DOWN ? 1 : 0
	 		));
	 		majDessin();
	 		break;

		case GLUT_KEY_PAGE_UP:
			if(modeCourant == MODE_APPEND) return;
			avancerSelectionSommetPoly(poly);
			majDessin();
			break;

		case GLUT_KEY_PAGE_DOWN:
			if(modeCourant == MODE_APPEND) return;
			reculerSelectionSommetPoly(poly);
			majDessin();
			break;
	}

}

void mouseButtonEvent(int button, int state, int x, int y) {
	
	if(button == GLUT_LEFT_BUTTON) {

		if(state == GLUT_DOWN) {

			switch(modeCourant) {
				case MODE_APPEND:
					break;
				case MODE_VERTEX:
					selectionnerPlusProcheSommetPoly(poly, positionSouris);
					if(approxDistancePoint(getPositionSommetSelection(poly), positionSouris) >= POLY_DISTANCE_SELECTION_MIN)
						return;
					dagEstActif = 1;
					break;
				case MODE_EDGE:
					break;
			}

		} else if(state == GLUT_UP) {

			switch(modeCourant) {
				case MODE_APPEND:
					ajouterSommetPoly(poly, newPoint(x, y));
					break;
				case MODE_VERTEX:
					dagEstActif = 0;
					break;
				case MODE_EDGE:
					selectionnerPlusProcheAretePoly(poly, positionSouris);
					break;
			}

		}		
		
		majDessin();

	} else if((button == GLUT_MIDDLE_BUTTON || button == 5) && state == GLUT_UP) {

		if(modeCourant == MODE_EDGE) {
			diviserAreteSelectionnePoly(poly);
			majDessin();
		}

	}
}

void activeMouseMoveEvent(int x, int y) {

	positionSouris = newPoint(x, y);

	if(modeCourant == MODE_VERTEX && dagEstActif) {
		deplacerSommetSelectionnePoly(poly, positionSouris);
		majDessin();
	} else if(modeCourant == MODE_APPEND)
		majDessin();

}

void passiveMouseMoveEvent(int x, int y) {

	positionSouris = newPoint(x, y);

	if(modeCourant == MODE_APPEND)
		majDessin();

}

int main(int argc, char **argv) {

	int l = 400, h = 400;

	if(argc == 3) {
		l = atoi(argv[1]);
		h = atoi(argv[2]);
		if(l <= 0 || h <= 0) {
			printf("Usage : %s [<largeur> <hauteur>]\n", argv[0]);
			exit(-1);
		}
	} else if(argc > 3 || argc == 2) {
		printf("Usage : %s [<largeur> <hauteur>]\n", argv[0]);
		exit(-1);
	}

	modeCourant = MODE_APPEND;
	zDessin = newZD(l, h);
	poly = newPoly();
	positionSouris = newPoint(0, 0);
	dagEstActif = aideAffichees = 0;

	glutInit(&argc, argv);
	glutInitWindowSize(zDessin->largeur, zDessin->hauteur);
	glutInitWindowPosition(
		glutGet(GLUT_SCREEN_WIDTH)/2 - zDessin->largeur/2,
		glutGet(GLUT_SCREEN_HEIGHT)/2 - zDessin->hauteur/2
	);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Projet FAIN");

	glViewport(0, 0, zDessin->largeur, zDessin->hauteur);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glOrtho(0, zDessin->largeur, zDessin->hauteur, 0, -1, 1);

	glutDisplayFunc(draw);
	glutKeyboardFunc(keyEvent);
	glutSpecialFunc(specialEvent);
	glutMouseFunc(mouseButtonEvent);
	glutMotionFunc(activeMouseMoveEvent);
	glutPassiveMotionFunc(passiveMouseMoveEvent);

	majDessin();

	glutMainLoop();

	return 0;
}