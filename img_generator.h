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
	img::EasyImage imgFromFigures_Wireframe(std::vector<lib3d::Figure>& figures, const double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor);
	img::EasyImage imgFromFigures_ZBufferWireframe(std::vector<lib3d::Figure>& figures, const double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor);
	img::EasyImage imgFromFigures_Triangles(std::vector<lib3d::Figure>& figures, double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor, std::vector<lib3d::Light>& lights, bool shadow = false, unsigned int shadowSize = 0);

	std::tuple<std::pair<int, int>, double, std::pair<double, double>> getImageFacts(std::vector<lib3d::Figure>& figures, double size, Matrix& eyeMatrix);
}
#endif