//
// Created by matthew
//

#ifndef ENGINE_MYLIBRARY_H
#define ENGINE_MYLIBRARY_H
const double MY_PI = 3.141592653589793238462643;
#include "easy_image.h"
#include <cmath>
#include <map>
#include <assert.h>
#include <algorithm>

#ifdef __linux__ 
#define EXTRA_PATH_IF_WINDOWS ""
#elif _WIN32
#define EXTRA_PATH_IF_WINDOWS "C:\\Users\\Matthew\\source\\repos\\Engine\\Debug\\"
#else
#define EXTRA_PATH_IF_WINDOWS ""
#endif

class Point2D {
public:
	double x;
	double y;

	Point2D(double x, double y);
	~Point2D();
};

class Line2D {
public:
	Point2D a;
	double z_a;

	Point2D b;
	double z_b;

	img::Color color;

	Line2D(Point2D& a, Point2D& b, img::Color& color);
	Line2D(Point2D& a, double z_a, Point2D& b, double z_b, img::Color& color);
	~Line2D();
};

inline double degreesToRad(double angle)
{
	return angle * MY_PI / 180.0;
};

inline int roundToInt(double d)
{
	return static_cast<int>(std::round(d));
};

std::tuple<int, int, double, double, double> propertiesOfImage(std::vector<Point2D>& points, const double size);

img::EasyImage imgFrom2DLines(std::vector<Line2D>& lines, std::vector<Point2D>& points, const double size, img::Color& colorBackground, bool UseZBuffering);

img::Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized);
#endif //ENGINE_MYLIBRARY_H
