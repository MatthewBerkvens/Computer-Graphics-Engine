//
// Created by matthew
//

#ifndef ENGINE_IMG_GENERATOR_H
#define ENGINE_IMG_GENERATOR_H
#include "easy_image.h"
#include "lib3d.h"
#include <cmath>
#include <map>
#include <assert.h>
#include <algorithm>

namespace img_generator
{
	img::EasyImage imgFrom2DLines(std::vector<Line2D>& lines, std::vector<Point2D>& points, const double size, img::Color& backgroundColor);
	img::EasyImage imgFromZBuffered2DLines(std::vector<Line2D>& lines, std::vector<Point2D>& points, const double size, img::Color& backgroundColor);
	img::EasyImage imgFromTriangleFigures(std::vector<lib3d::Figure>& figures, double size, img::Color& backgroundColor);
}
#endif