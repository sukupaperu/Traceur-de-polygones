#include "Couleur.h"
#include <math.h>
#include <stdio.h>

Couleur newCouleur(float r, float g, float b) {
    return (Couleur){r, g, b};
}

Couleur hsvVersCouleur(float h, float s, float v) {
    int i = floor(h*6);
    float r, g, b, f = h*6 - i, p = v*(1 - s), q = v*(1 - f*s), t = v*(1 - (1 - f)*s);
    
    switch (i%6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return newCouleur(r, g, b);
}