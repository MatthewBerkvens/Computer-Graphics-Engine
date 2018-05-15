#include "Projecting.h"

Point2D projectPoint(const Vector3D& point, const double d, const double dx, const double dy)
{
	return Point2D((d* (point.x / -point.z)) + dx, (d*(point.y / -point.z)) + dy);
}