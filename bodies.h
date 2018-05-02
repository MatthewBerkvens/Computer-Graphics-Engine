//
// Created by matthew
//

#ifndef ENGINE_BODIES_H
#define ENGINE_BODIES_H
#include "easy_image.h"
#include "lib3d.h"

namespace bodies {
	void createCube(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createTetrahedron(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createOctahedron(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createIcosahedron(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createDodecahedron(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createCone(lib3d::Figure& figure, const unsigned int n, const double h, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createCylinder(lib3d::Figure& figure, const unsigned int n, const double h, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient, bool surfaces = true);

	void createSphere(lib3d::Figure& figure, const unsigned int n, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void createBuckyBall(lib3d::Figure& figure, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);

	void generateThickFigure(lib3d::Figure& lineDrawing, std::vector<lib3d::Figure>& figures, const double r, const int n, const int m);
}
#endif