//
// Created by matthew
//

#ifndef ENGINE_FACE_H
#define ENGINE_FACE_H

#include "vector3d.h"
#include <vector>

class Face
{
public:
	std::vector<unsigned int> point_indexes;

	Face(std::vector<unsigned int> _point_indexes);
	Face();
};

#endif