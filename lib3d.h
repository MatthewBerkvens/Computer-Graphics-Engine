//
// Created by matthew
//

#ifndef ENGINE_LIB3D_H
#define ENGINE_LIB3D_H
#include "vector3d.h"
#include "mylibrary.h"
#include "Figure.h"

#include <vector>
#include <assert.h>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace lib3d
{
	Matrix scaleMatrix(const double scale);

	Matrix rotateXMatrix(const double angle);

	Matrix rotateYMatrix(const double angle);

	Matrix rotateZMatrix(const double angle);

	Matrix translateMatrix(const Vector3D& vector);

	Matrix transformEyePointMatrix(const Vector3D& eyepoint);

	std::tuple<double, double, double> toPolar(const Vector3D& point);



	void transformFigure(Figure& figure, const Matrix& transformMatrix);

	void applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint);
}

#endif //ENGINE_LIB3D_H
