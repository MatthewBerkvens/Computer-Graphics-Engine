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

    double x_0of3 = figure.points[0].x;
    double x_1of3 = figure.points[0].x + ((figure.points[5].x - figure.points[0].x) / 3);
    double x_2of3 = figure.points[0].x + ((figure.points[5].x - figure.points[0].x) *  2 / 3);

    double y_0of3 = figure.points[0].y;
    double y_1of3 = figure.points[0].y + ((figure.points[4].y - figure.points[0].y) / 3);
    double y_2of3 = figure.points[0].y + ((figure.points[4].y - figure.points[0].y) *  2 / 3);

    double z_0of3 = figure.points[0].z;
    double z_1of3 = figure.points[0].z + ((figure.points[6].z - figure.points[0].z) / 3);
    double z_2of3 = figure.points[0].z + ((figure.points[6].z - figure.points[0].z) *  2 / 3);

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                if ((x == 1 && y == 1) || (x == 1 && z == 1) || (y == 1 && z == 1)) continue;

                lib3d::Figure newFigure = lib3d::Figure(figure);
                lib3d::transformFigure(newFigure, scaleMatrix);

                double vector_x = x < 2 ? (x < 1 ? x_0of3 : x_1of3) : x_2of3;
                double vector_y = y < 2 ? (y < 1 ? y_0of3 : y_1of3) : y_2of3;
                double vector_z = z < 2 ? (z < 1 ? z_0of3 : z_1of3) : z_2of3;

                Vector3D vector_combined = Vector3D().vector(vector_x, vector_y, vector_z);

                lib3d::transformFigure(newFigure, lib3d::translateMatrix(Vector3D().vector(newFigure.points[0]) - vector_combined));
                fractals::recursiveMengerSponge(newFigure, fractal, scaleMatrix, iter + 1, max_iter);
            }
        }
    }
}
