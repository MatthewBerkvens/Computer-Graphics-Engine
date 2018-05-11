#include "Color.h"


Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized)
{
	if (colorNormalized.size() != 3)
		throw std::invalid_argument("expected size 3");
	return Color((uint8_t)roundToInt(colorNormalized[0] * 255), (uint8_t)roundToInt(colorNormalized[1] * 255), (uint8_t)roundToInt(colorNormalized[2] * 255));
}


Color::Color()
	: blue(0), green(0), red(0)
{

}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
	: blue(b), green(g), red(r)
{

}

Color::~Color()
{

}