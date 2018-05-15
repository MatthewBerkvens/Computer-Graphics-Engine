//
// Created by matthew
//

#ifndef ENGINE_PROJECTING_H
#define ENGINE_PROJECTING_H
#include "vector3d.h"
#include "Point2D.h"

Point2D projectPoint(const Vector3D& point, const double d, const double dx = 0, const double dy = 0);
#endif