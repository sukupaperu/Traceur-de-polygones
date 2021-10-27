#ifndef _COULEUR_H_
#define _COULEUR_H_

typedef struct {
    float r;
    float g;
    float b;
} Couleur;

Couleur newCouleur(float, float, float);
Couleur hsvVersCouleur(float, float, float);

#endif