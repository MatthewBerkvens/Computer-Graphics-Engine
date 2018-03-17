//
// Created by matthew
//

#ifndef ENGINE_WIREFRAME_H
#define ENGINE_WIREFRAME_H
#include "easy_image.h"
#include "lib3d.h"
#include "ini_configuration.h"

namespace wireframe {
	img::EasyImage generate_Wireframe(const ini::Configuration& conf, bool UseZBuffering);

	void parseLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name);

	void createCube(lib3d::Figure& figure);

	void createTetrahedron(lib3d::Figure& figure);

	void createOctahedron(lib3d::Figure& figure);

	void createIcosahedron(lib3d::Figure& figure);

	void createDodecahedron(lib3d::Figure& figure);

	void createCone(lib3d::Figure& figure, const unsigned int n, const double h);

	void createCylinder(lib3d::Figure& figure, const unsigned int n, const double h);

	void createSphere(lib3d::Figure& figure, const unsigned int n);

	void createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m);
}
#endif //ENGINE_WIREFRAME_H