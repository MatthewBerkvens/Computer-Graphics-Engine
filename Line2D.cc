#include "Line2D.h"

Line2D::Line2D(Point2D& a, Point2D& b, Color& color)
	: a(a), b(b), color(color)
{

}

Line2D::Line2D(Point2D& a, double z_a, Point2D& b, double z_b, Color& color)
	: a(a), z_a(z_a), b(b), z_b(z_b), color(color)
{

}