#include "Point.h"

Point newPoint(int x, int y) {
	return (Point){x, y};
}

int valeurEstEncadree(int a, int valeur, int b) {
	return (a >= valeur && valeur > b) || (b >= valeur && valeur > a);
}

int approxDistancePoint(Point a, Point b) {
	Point c = newPoint(b.x - a.x, b.y - a.y);
	return c.x*c.x + c.y*c.y;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int approxDistanceSegment(Point A, Point B, Point C) {
	if(A.x == B.x) {
		if(valeurEstEncadree(A.y, C.y, B.y))
			return approxDistancePoint(newPoint(A.x, 0), newPoint(C.x, 0));
	} else if(A.y == B.y) {
		if(valeurEstEncadree(A.x, C.x, B.x))
			return approxDistancePoint(newPoint(0, A.y), newPoint(0, C.y));
	} else {
		float a = (float)(B.y - A.y)/(float)(B.x - A.x);
		float b = A.y - a*(float)A.x;

		float aa = -1/a;
		float bb = C.y + (float)C.x/a;

		float xx = (b - bb)/(aa - a);
		Point I = newPoint(xx, a*xx + b);

		if(valeurEstEncadree(A.x, I.x, B.x))
			return approxDistancePoint(I, C);
	}
	return min(approxDistancePoint(A, C), approxDistancePoint(B, C));
}