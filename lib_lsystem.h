//
// Created by matthew
//

#ifndef LIB_LSYSTEM_INCLUDED
#define LIB_LSYSTEM_INCLUDED
#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "mylibrary.h"
#include "Line2D.h"
#include "Point2D.h"
#include "Figure.h"
#include "img_generator.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <algorithm>

namespace lib_lsystem {
	img::EasyImage generate_2DLSystem(const ini::Configuration &conf);

	void generate_3DLSystem(Figure& figure, const ini::Configuration &conf, std::string figurename);

	void LSystem2DIterate(std::vector<Line2D>& lines,
		std::vector<Point2D>& points,
		LParser::LSystem2D& l_system,
		std::vector<std::tuple<double, double, double >> &stack,
		double& current_x,
		double& current_y,
		double& angle,
		Color& color,
		char& chr,
		unsigned int iter);

	void LSystem3DIterate(Figure& figure,
		LParser::LSystem3D& l_system,
		std::vector<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D>>& stack,
		Vector3D& pos,
		Vector3D& H,
		Vector3D& L,
		Vector3D& U,
		double& angleInRad,
		char& chr,
		unsigned int iter);
}

#endif //LIB_LSYSTEM_INCLUDED
