//
// Created by matthew
//

#ifndef ENGINE_BODIES_H
#define ENGINE_BODIES_H
#include "easy_image.h"
#include "lib3d.h"

namespace bodies {
	void createCube(lib3d::Figure& figure);

	void createTetrahedron(lib3d::Figure& figure);

	void createOctahedron(lib3d::Figure& figure);

	void createIcosahedron(lib3d::Figure& figure);

	void createDodecahedron(lib3d::Figure& figure);

	void createCone(lib3d::Figure& figure, const unsigned int n, const double h);

	void createCylinder(lib3d::Figure& figure, const unsigned int n, const double h);

	void createSphere(lib3d::Figure& figure, const unsigned int n);

	void createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m);

	void createBuckyBall(lib3d::Figure& figure);

	void createMengerSponge(lib3d::Figure& figure, const unsigned int nr_iterations);
}
#endif