//
// Created by matthew
//

#ifndef ENGINE_FRACTALS_H
#define ENGINE_FRACTALS_H
#include "lib3d.h"
#include "bodies.h"

namespace fractals {
	void createFractal(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const unsigned int nr_iterations, const double scale);

	void recursiveFractal(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter);

	void createMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const unsigned int nr_iterations);

	void recursiveMengerSponge(lib3d::Figure& figure, std::vector<lib3d::Figure>& fractal, const Matrix& scaleMatrix, const unsigned int iter, const unsigned int max_iter);
}

#endif