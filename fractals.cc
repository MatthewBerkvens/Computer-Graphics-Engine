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



void fractals::createMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const unsigned int nr_iterations)
{
	Matrix scaleMatrix = lib3d::scaleMatrix(1.0 / 3.0);

	recursiveMengerSponge(figure, fractal, scaleMatrix, 0, nr_iterations);

	/*for (size_t i = 0; i < fractal.size(); i++)
	{
		for (size_t x = 0; x < fractal[i].points.size(); x++)
		{
			if (fractal[i].points[x].x == -1)
				continue;
		}
	}*/
}


void fractals::recursiveMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter)
{
	if (iter >= max_iter)
	{
		fractal.push_back(figure);
		return;
	}

	std::vector<lib3d::Figure> newFigures;

	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			for (int z = -1; z < 2; z++)
			{
				if ((x == 0 && y == 0) || (x == 0 && z == 0) || (y == 0 && z == 0)) continue;

				lib3d::Figure newFigure = lib3d::Figure(figure);
				lib3d::transformFigure(newFigure, scaleMatrix);

				lib3d::transformFigure(newFigure, lib3d::translateMatrix(Vector3D().vector(x * (2.0 / 3.0), y * (2.0 / 3.0), z * (2.0 / 3.0))));

				newFigures.push_back(newFigure);
			}
		}
	}

	lib3d::Figure combinedFigure = lib3d::combineFigures(newFigures);

	fractals::recursiveMengerSponge(combinedFigure, fractal, scaleMatrix, iter + 1, max_iter);
}