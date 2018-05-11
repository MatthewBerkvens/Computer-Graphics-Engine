//
// Created by matthew on 06.03.18.
//

#include "lib3d.h"

Matrix lib3d::scaleMatrix(const double scale)
{
	Matrix matrix;
	matrix(1, 1) = scale;
	matrix(2, 2) = scale;
	matrix(3, 3) = scale;
	return matrix;
}

Matrix lib3d::rotateXMatrix(const double angle)
{
	Matrix matrix;
	matrix(2, 2) = std::cos(angle);
	matrix(2, 3) = std::sin(angle);
	matrix(3, 2) = -std::sin(angle);
	matrix(3, 3) = std::cos(angle);
	return matrix;
}

Matrix lib3d::rotateYMatrix(const double angle)
{
	Matrix matrix;
	matrix(1, 1) = std::cos(angle);
	matrix(1, 3) = -std::sin(angle);
	matrix(3, 1) = std::sin(angle);
	matrix(3, 3) = std::cos(angle);
	return matrix;
}

Matrix lib3d::rotateZMatrix(const double angle)
{
	Matrix matrix;
	matrix(1, 1) = std::cos(angle);
	matrix(1, 2) = std::sin(angle);
	matrix(2, 1) = -std::sin(angle);
	matrix(2, 2) = std::cos(angle);
	return matrix;
}

Matrix lib3d::translateMatrix(const Vector3D& vector)
{
	assert(vector.is_vector());
	Matrix matrix;
	matrix(4, 1) = vector.x;
	matrix(4, 2) = vector.y;
	matrix(4, 3) = vector.z;
	return matrix;
}

Matrix lib3d::transformEyePointMatrix(const Vector3D& eyepoint)
{
	Matrix matrix;

	std::tuple<double, double, double> polar = lib3d::toPolar(eyepoint);

	double r = std::get<0>(polar);
	double theta = std::get<1>(polar);
	double phi = std::get<2>(polar);

	matrix(1, 1) = -std::sin(theta);
	matrix(1, 2) = -std::cos(theta) * std::cos(phi);
	matrix(1, 3) = std::cos(theta) * std::sin(phi);

	matrix(2, 1) = std::cos(theta);
	matrix(2, 2) = -std::sin(theta) * std::cos(phi);
	matrix(2, 3) = std::sin(theta) * std::sin(phi);

	matrix(3, 2) = std::sin(phi);
	matrix(3, 3) = std::cos(phi);

	matrix(4, 3) = -r;

	return matrix;
}

std::tuple<double, double, double> lib3d::toPolar(const Vector3D& point)
{
	double r = point.length();
	double theta = std::atan2(point.y, point.x);
	double phi = std::acos(point.z / r);
	return std::tuple<double, double, double>(r, theta, phi);
}

void lib3d::transformFigure(Figure& figure, const Matrix& transformMatrix)
{
	for (std::vector<Vector3D>::iterator it = figure.points.begin(); it != figure.points.end(); ++it)
	{
		(*it) *= transformMatrix;
	}
}

void lib3d::applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint)
{
	Matrix eyePointMatrix = transformEyePointMatrix(eyepoint);

	for (std::vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it)
	{
		transformFigure(*it, eyePointMatrix);
	}
}