//
// Created by matthew
//

#ifndef ENGINE_FRACTALS_H
#define ENGINE_FRACTALS_H
#include "lib3d.h"
#include "bodies.h"
#include "Projecting.h"

namespace fractals {
	void createFractal(Figure& figure, std::vector<Figure>& fractal, const unsigned int nr_iterations, const double scale);

	void recursiveFractal(Figure& figure, std::vector<Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter);

	void createMengerSponge(Figure& figure, std::vector<Figure>& fractal, const unsigned int nr_iterations);

	void recursiveMengerSponge(Figure& figure, std::vector<Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter);
}

#endif