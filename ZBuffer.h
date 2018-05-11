//
// Created by matthew
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>

class ZBuffer
{
public:
	std::vector<std::vector<double>> zbuffer;

	ZBuffer(const unsigned int width, const unsigned int height);

	std::vector<double>& operator[](const unsigned int i);
};

#endif