//
// Created by matthew.
//

#include "mylibrary.h"

Point2D::Point2D(double x, double y) : x(x), y(y) {}

Point2D::~Point2D() {}

Line2D::Line2D(Point2D& a, Point2D& b, img::Color& color) : a(a), b(b), color(color) {}
Line2D::Line2D(Point2D& a, double z_a, Point2D& b, double z_b, img::Color& color) : a(a), z_a(z_a), b(b), z_b(z_b), color(color) {}

Line2D::~Line2D() {}

img::Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized)
{
	if (colorNormalized.size() != 3)
		throw std::invalid_argument("received vector with incorrect size for color, expected size 3.");
	return img::Color((int)(colorNormalized[0] * 255), (int)(colorNormalized[1] * 255), (int)(colorNormalized[2] * 255));
}