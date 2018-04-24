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
}


void fractals::recursiveMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter)
{
	if (iter >= max_iter)
	{
		fractal.push_back(figure);
		return;
	}

    double x = (figure.points[0] - figure.points[5]).length();

    double y = (figure.points[0] - figure.points[4]).length();

    double z = (figure.points[0] - figure.points[6]).length();

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            for (int z = 0; z < 3; z++)
            {
                if ((x == 1 && y == 1) || (x == 1 && z == 1) || (y == 1 && z == 1)) continue;

                lib3d::Figure newFigure = lib3d::Figure(figure);
                lib3d::transformFigure(newFigure, scaleMatrix);

                double pt_x = x < 2 ? (x < 1 ? x/3.0 : 0) : -x/3.0;
                double pt_y = y < 2 ? (y < 1 ? y/3.0 : 0) : -y/3.0;
                double pt_z = z < 2 ? (z < 1 ? z/3.0 : 0) : -z/3.0;

                std::cout << pt_x << " " << pt_y << " " << pt_z << std::endl;

                lib3d::transformFigure(newFigure, lib3d::translateMatrix(Vector3D().vector(pt_x, pt_y, pt_z)));
                fractals::recursiveMengerSponge(newFigure, fractal, scaleMatrix, iter + 1, max_iter);
            }
        }
    }
}
