//
// Created by matthew
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "vector3d.h"
#include "ZBuffer.h"
#include "Point2D.h"
#include "mylibrary.h"

#include <algorithm>
#include <vector>
#include <assert.h>

class Light
{
public:
	std::vector<double> ambientLight;
	std::vector<double> diffuseLight;
	std::vector<double> specularLight;

	bool specialLight;
	bool infinity;

	ZBuffer shadowmask;
	Matrix lightAsEyeMatrix;
	double d;
	double dx;
	double dy;

	Vector3D ldVector;
	Vector3D location;

	Light(std::vector<double>& _ambientLight, std::vector<double>& _diffuseLight, std::vector<double>& _specularLight, bool _specialLight, bool _infinity, Vector3D& _ldVector, Vector3D& _location, Matrix _lightAsEyeMatrix = Matrix());

	void addShadowTriangle(const Vector3D& A, const Vector3D& B, const Vector3D& C);

	bool isInSight(Vector3D& realWorldPoint);
};

#endif