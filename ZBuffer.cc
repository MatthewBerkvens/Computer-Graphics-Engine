#include "ZBuffer.h"

ZBuffer::ZBuffer(const unsigned int width, const unsigned int height)
{
	double posInf = std::numeric_limits<double>::infinity();

	zbuffer = std::vector<std::vector<double>>(width, std::vector<double>(height, posInf));
}

std::vector<double>& ZBuffer::operator[](const unsigned int i)
{
	return zbuffer[i];
}