//
// Created by matthew
//

#ifndef ENGINE_PROJECTING_H
#define ENGINE_PROJECTING_H
#include "vector3d.h"
#include "Point2D.h"
#include "Color.h"
#include "Line2D.h"
#include "Figure.h"

#include <vector>
#include <assert.h>

std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d);

void combineFigures(Figure& out, std::vector<Figure>& figures);

Point2D projectPoint(const Vector3D& point, const double d, const double dx = 0, const double dy = 0);
#endif