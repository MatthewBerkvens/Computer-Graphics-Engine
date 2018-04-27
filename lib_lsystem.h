//
// Created by matthew
//

#ifndef LIB_LSYSTEM_INCLUDED
#define LIB_LSYSTEM_INCLUDED
#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "lib3d.h"
#include "mylibrary.h"

using namespace std;

namespace lib_lsystem {
    img::EasyImage generate_2DLSystem(const ini::Configuration &conf);

	lib3d::Figure generate_3DLSystem(lib3d::Figure& figure, const ini::Configuration &conf, string figurename, img::Color& color);

	void LSystem2DIterate(vector<Line2D>& lines,
		vector<Point2D>& points,
		LParser::LSystem2D& l_system,
		vector<tuple<double, double, double>>& stack,
		double& current_x,
		double& current_y, 
		double& angle,
		img::Color& color,
		char& chr,
		unsigned int iter);

	void LSystem3DIterate(lib3d::Figure& figure,
		LParser::LSystem3D& l_system,
		vector<tuple<Vector3D, Vector3D, Vector3D, Vector3D>>& stack,
		Vector3D& pos,
		Vector3D& H,
		Vector3D& L,
		Vector3D& U,
		double& angleInRad,
		char& chr,
		img::Color& color,
		unsigned int iter);
}

#endif //LIB_LSYSTEM_INCLUDED
