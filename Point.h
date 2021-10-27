#ifndef _POINT_H_
#define _POINT_H_

typedef struct {
    int x;
    int y;
} Point;

Point newPoint(int, int);
int valeurEstEncadree(int, int, int);
int approxDistancePoint(Point, Point);
int approxDistanceSegment(Point, Point, Point);

#endif