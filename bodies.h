//
// Created by matthew
//

#ifndef ENGINE_BODIES_H
#define ENGINE_BODIES_H
#include "easy_image.h"
#include "lib3d.h"

namespace bodies {
	void createCube(lib3d::Figure& figure, img::Color& color);

	void createTetrahedron(lib3d::Figure& figure, img::Color& color);

	void createOctahedron(lib3d::Figure& figure, img::Color& color);

	void createIcosahedron(lib3d::Figure& figure, img::Color& color);

	void createDodecahedron(lib3d::Figure& figure, img::Color& color);

	void createCone(lib3d::Figure& figure, const unsigned int n, const double h, img::Color& color);

	void createCylinder(lib3d::Figure& figure, const unsigned int n, const double h, img::Color& color);

	void createSphere(lib3d::Figure& figure, const unsigned int n, img::Color& color);

	void createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m, img::Color& color);

	void createBuckyBall(lib3d::Figure& figure, img::Color& color);
}
#endif