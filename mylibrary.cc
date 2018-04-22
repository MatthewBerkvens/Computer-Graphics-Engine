//
// Created by matthew.
//

#include "mylibrary.h"
#include <assert.h>
#include <algorithm>

Point2D::Point2D(double x, double y) : x(x), y(y) {}

Point2D::~Point2D() {}

Line2D::Line2D(Point2D& a, Point2D& b, img::Color& color) : a(a), b(b), color(color) {}
Line2D::Line2D(Point2D& a, double z_a, Point2D& b, double z_b, img::Color& color) : a(a), z_a(z_a), z_b(z_b), b(b), color(color) {}

Line2D::~Line2D() {}

img::Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized)
{
	if (colorNormalized.size() != 3)
		throw std::invalid_argument("received vector with incorrect size for color, expected size 3.");
	return img::Color((int)(colorNormalized[0] * 255), (int)(colorNormalized[1] * 255), (int)(colorNormalized[2] * 255));
}


img::EasyImage imgFrom2DLines(std::vector<Line2D>& lines, std::vector<Point2D>& points, const double size, img::Color& backgroundColor, bool UseZBuffering)
{
	auto minmax_x = std::minmax_element(points.begin(), points.end(), [](const Point2D &a, const Point2D &b) {
		return a.x < b.x;
	});
	auto minmax_y = std::minmax_element(points.begin(), points.end(), [](const Point2D &a, const Point2D &b) {
		return a.y < b.y;
	});

	double min_x = minmax_x.first->x;
	double max_x = minmax_x.second->x;

	double min_y = minmax_y.first->y;
	double max_y = minmax_y.second->y;


	double range_x = max_x - min_x;
	double range_y = max_y - min_y;

	double image_x = ((double)size) * range_x / std::max(range_x, range_y);
	double image_y = ((double)size) * range_y / std::max(range_x, range_y);

	double d = 0.95 * (image_x / range_x);

	img::EasyImage image = img::EasyImage(roundToInt(image_x), roundToInt(image_y), backgroundColor);

	double offset_x = (image_x / 2.0) - (d*(min_x + max_x) / 2.0);
	double offset_y = (image_y / 2.0) - (d*(min_y + max_y) / 2.0);

	if (UseZBuffering)
	{
		img::ZBuffer zbuffer = img::ZBuffer(roundToInt(image_x), roundToInt(image_y));

		for (std::vector<Line2D>::iterator it = lines.begin(); it != lines.end(); ++it)
		{
			image.draw_zbuf_line(
				zbuffer,
				roundToInt((d * it->a.x) + offset_x),
				roundToInt((d * it->a.y) + offset_y),
				it->z_a,
				roundToInt((d * it->b.x) + offset_x),
				roundToInt((d * it->b.y) + offset_y),
				it->z_b,
				it->color);
		}
	}
	else
	{
		for (std::vector<Line2D>::iterator it = lines.begin(); it != lines.end(); ++it)
		{
			image.draw_line(
				roundToInt((d * it->a.x) + offset_x),
				roundToInt((d * it->a.y) + offset_y),
				roundToInt((d * it->b.x) + offset_x),
				roundToInt((d * it->b.y) + offset_y),
				it->color);
		}
	}

	return image;
}