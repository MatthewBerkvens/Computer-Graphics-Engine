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
	img::EasyImage imgFrom2DLines(std::vector<lib3d::Line2D>& lines, std::vector<lib3d::Point2D>& points, const double size, lib3d::Color& backgroundColor);
	img::EasyImage imgFromZBuffered2DLines(std::vector<lib3d::Line2D>& lines, std::vector<lib3d::Point2D>& points, const double size, lib3d::Color& backgroundColor);
	img::EasyImage imgFromTriangleFigures(std::vector<lib3d::Figure>& figures, double size, lib3d::Color& backgroundColor, std::vector<lib3d::Light>& lights, bool shadow = false, unsigned int shadowSize = 0, Matrix eyeMatrix = Matrix());
}
#endif