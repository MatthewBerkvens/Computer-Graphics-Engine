//
// Created by matthew
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H

#include "vector3d.h"
#include "Face.h"
#include "easy_image.h"

#include <vector>
#include <assert.h>


class Figure
{
public:
	std::vector<Vector3D> points;
	std::vector<Face> faces;

	std::vector<double> ambientReflection;
	std::vector<double> diffuseReflection;
	std::vector<double> specularReflection;
	double reflectionCoefficient;

	img::EasyImage texture;
	unsigned int textureMethod;
	std::vector<std::vector<Vector3D>> surfaceInformation;

	Figure(std::vector<double>& _ambientReflection, std::vector<double>& _diffuseReflection, std::vector<double>& _specularReflection, double _reflectionCoefficient);
};

std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d);

void combineFigures(Figure& out, std::vector<Figure>& figures);
#endif