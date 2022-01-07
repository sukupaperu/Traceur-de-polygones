#include "Point.h"

Point newPoint(int x, int y) {
	return (Point){x, y};
}

// indique si 'valeur' est compris entre A et B (ou entre B et A)
int valeurEstEncadree(int a, int valeur, int b) {
	return (a >= valeur && valeur > b) || (b >= valeur && valeur > a);
}

// distance "presque" eunclidienne
// (sans la racine carrée dont on peut se passer puisqu'on n'a pas besoin de la distance exacte)
int approxDistancePoint(Point a, Point b) {
	Point c = newPoint(b.x - a.x, b.y - a.y);
	return c.x*c.x + c.y*c.y;
}

int min(int a, int b) {
	return a < b ? a : b;
}

// renvoie la distance approximative (presque euclidienne, mais sans la racine carrée)
// entre le point C, et le segment AB
int approxDistanceSegment(Point A, Point B, Point C) {
	// si AB est vertical
	if(A.x == B.x) {
		// si B la projeté orthogonal de C est bien sur le segment AB
		if(valeurEstEncadree(A.y, C.y, B.y))
			return approxDistancePoint(newPoint(A.x, 0), newPoint(C.x, 0));
	} 
	// si AB est horizontal
	else if(A.y == B.y) {
		// si B la projeté orthogonal de C est bien sur le segment AB
		if(valeurEstEncadree(A.x, C.x, B.x))
			return approxDistancePoint(newPoint(0, A.y), newPoint(0, C.y));
	}
	// si AB est ni vertical, ni horizontal (dans les cas précédents on aurait eu des soucis de division par 0)
	else {
		// f(x) = a*x + b, la droite affine passant par A et B
		float a = (float)(B.y - A.y)/(float)(B.x - A.x); // pente de f(x)
		float b = A.y - a*(float)A.x; // ordonnée à l'origine de f(x)

		// g(x) = aa*x + bb, la droite affine passant par C et perpendiculaire à f(x)
		float aa = -1/a; // pente de g(x)
		float bb = C.y + (float)C.x/a; // ordonnée à l'origine de g(x)

		// on calcule le point I qui est est situé à l'intersection de f(x) et g(x)
		float iX = (b - bb)/(aa - a); // position X de l'intersection
		Point I = newPoint(iX, a*iX + b); // I = (iX, f(iX)) = (iX, a*iX + b)

		// si B la projeté orthogonal de C est bien sur le segment AB
		if(valeurEstEncadree(A.x, I.x, B.x))
			return approxDistancePoint(I, C);
	}
	// si on arrive ici, c'est que le projeté orthogonal de C n'est pas sur le segment AB
	// auquel cas on doit juste calculer la valeur minimale entre les distances AC et BC
	return min(approxDistancePoint(A, C), approxDistancePoint(B, C));
}