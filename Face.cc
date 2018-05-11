#include "Face.h"

Face::Face(std::vector<unsigned int> _point_indexes)
	: point_indexes(_point_indexes)
{

}

Face::Face()
	: point_indexes({})
{

}