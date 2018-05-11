//
// Created by matthew
//

#ifndef ENGINE_BODIES_H
#define ENGINE_BODIES_H

#include "mylibrary.h"
#include "Figure.h"
#include "Face.h"
#include "lib3d.h"

#include <algorithm>
#include <assert.h>

namespace bodies {
	void createCube(Figure& figure);

	void createTetrahedron(Figure& figure);

	void createOctahedron(Figure& figure);

	void createIcosahedron(Figure& figure);

	void createDodecahedron(Figure& figure);

	void createCone(Figure& figure, const unsigned int n, const double h);

	void createCylinder(Figure& figure, const unsigned int n, const double h, bool surfaces = true);

	void createSphere(Figure& figure, const unsigned int n);

	void createTorus(Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m);

	void createBuckyBall(Figure& figure);

	void generateThickFigure(Figure& lineDrawing, std::vector<Figure>& figures, const double r, const int n, const int m);
}
#endif