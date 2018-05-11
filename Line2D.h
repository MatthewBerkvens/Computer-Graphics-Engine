//
// Created by matthew
//

#ifndef ENGINE_LINE2D_H
#define ENGINE_LINE2D_H

#include "Color.h"
#include "Point2D.h"

class Line2D {
public:
	Point2D a;
	double z_a;

	Point2D b;
	double z_b;

	Color color;

	Line2D(Point2D& a, Point2D& b, Color& color);
	Line2D(Point2D& a, double z_a, Point2D& b, double z_b, Color& color);
};

#endif