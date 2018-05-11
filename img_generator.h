//
// Created by matthew
//

#ifndef ENGINE_IMG_GENERATOR_H
#define ENGINE_IMG_GENERATOR_H
#include "easy_image.h"
#include "Point2D.h"
#include "Line2D.h"
#include "Light.h"
#include "lib3d.h"

#include <cmath>
#include <map>
#include <assert.h>
#include <algorithm>


namespace img_generator
{
	img::EasyImage imgFrom2DLines(std::vector<Line2D>& lines, std::vector<Point2D>& points, const double size, Color& backgroundColor);
	img::EasyImage imgFromFigures_Wireframe(std::vector<Figure>& figures, const double size, Matrix& eyeMatrix, Color& backgroundColor);
	img::EasyImage imgFromFigures_ZBufferWireframe(std::vector<Figure>& figures, const double size, Matrix& eyeMatrix, Color& backgroundColor);
	img::EasyImage imgFromFigures_Triangles(std::vector<Figure>& figures, double size, Matrix& eyeMatrix, Color& backgroundColor, std::vector<Light>& lights, bool shadow = false, unsigned int shadowSize = 0);

	std::tuple<std::pair<int, int>, double, std::pair<double, double>> getImageFacts(std::vector<Figure>& figures, double size, Matrix& eyeMatrix);
}
#endif