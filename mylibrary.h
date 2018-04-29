//
// Created by matthew
//

#ifndef ENGINE_MYLIBRARY_H
#define ENGINE_MYLIBRARY_H
const double MY_PI = 3.141592653589793238462643;
#include <cmath>

#ifdef __linux__ 
#define EXTRA_PATH_IF_WINDOWS ""
#elif _WIN32
#define EXTRA_PATH_IF_WINDOWS "C:\\Users\\Matthew\\source\\repos\\Engine\\Debug\\"
#else
#define EXTRA_PATH_IF_WINDOWS ""
#endif

inline double degreesToRad(double angle)
{
	return angle * MY_PI / 180.0;
};

inline int roundToInt(double d)
{
	return static_cast<int>(std::round(d));
};
#endif