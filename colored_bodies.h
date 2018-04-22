//
// Created by matthew
//

#ifndef ENGINE_COLORED_BODIES_H
#define ENGINE_COLORED_BODIES_H
#include "bodies.h"
#include "lib3d.h"
#include "ini_configuration.h"

namespace colored_bodies {
	img::EasyImage generate_ColoredBodies(const ini::Configuration& conf);

	void triangulate(std::vector<lib3d::Face>& out, const lib3d::Face& in);

	img::EasyImage imgFromTriangleFigures(std::vector<lib3d::Figure>& figures, const double size, img::Color& backgroundColor);
}

#endif