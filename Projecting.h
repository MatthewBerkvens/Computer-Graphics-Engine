//
// Created by matthew
//

#ifndef ENGINE_PROJECTING_H
#define ENGINE_PROJECTING_H

#include "Color.h"
#include "Point2D.h"
#include "Line2D.h"
#include "Figure.h"

#include <vector>
#include <assert.h>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>

std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d);

void combineFigures(Figure& out, std::vector<Figure>& figures);

#endif