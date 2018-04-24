//
// Created by matthew
//

#include "fractals.h"

void fractals::createFractal(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const unsigned int nr_iterations, const double scale)
{
	Matrix scaleMatrix = lib3d::scaleMatrix(1 / scale);

	recursiveFractal(figure, fractal, scaleMatrix, 0, nr_iterations);
}

void fractals::recursiveFractal(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter)
{
	if (iter >= max_iter)
	{
		fractal.push_back(figure);
		return;
	}

	for (size_t i = 0; i < figure.points.size(); i++)
	{
		lib3d::Figure newFigure = lib3d::Figure(figure);
		lib3d::transformFigure(newFigure, scaleMatrix);
		Matrix translateMatrix = lib3d::translateMatrix((Vector3D().vector(figure.points[i]) - Vector3D().vector(newFigure.points[i])));
		lib3d::transformFigure(newFigure, translateMatrix);
		recursiveFractal(newFigure, fractal, scaleMatrix, iter + 1, max_iter);
	}
}



void fractals::createMengerSponge(std::vector<lib3d::Figure>& fractal, const unsigned int nr_iterations)
{
	lib3d::Figure cube;
	bodies::createCube(cube);

	Matrix scaleMatrix = lib3d::scaleMatrix(1 / 3);

	recursiveMengerSponge(cube, fractal, scaleMatrix, 0, nr_iterations);
}


void fractals::recursiveMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter)
{
	if (iter >= max_iter)
	{
		fractal.push_back(figure);
		return;
	}

	for (size_t i = 0; i < figure.points.size(); i++)
	{
		lib3d::Figure newFigure = lib3d::Figure(figure);
		lib3d::transformFigure(newFigure, scaleMatrix);
		Matrix translateMatrix = lib3d::translateMatrix((Vector3D().vector(figure.points[i]) - Vector3D().vector(newFigure.points[i])));
		lib3d::transformFigure(newFigure, translateMatrix);
		recursiveFractal(newFigure, fractal, scaleMatrix, iter + 1, max_iter);
	}


	lib3d::Figure newFigure = lib3d::Figure(figure);
	lib3d::transformFigure(newFigure, scaleMatrix);
	Matrix translateMatrix = lib3d::translateMatrix(Vector3D().vector(figure.points[i]) - Vector3D().vector(newFigure.points[i]));
	lib3d::transformFigure(newFigure, translateMatrix);
	recursiveFractal(newFigure, fractal, scaleMatrix, iter + 1, max_iter);
}
