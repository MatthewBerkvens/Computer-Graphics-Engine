#include "Figure.h"

Figure::Figure(std::vector<double>& _ambientReflection, std::vector<double>& _diffuseReflection, std::vector<double>& _specularReflection, double _reflectionCoefficient, img::EasyImage _texture)
	: ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient), texture(_texture)
{
	assert(_ambientReflection.size() == 3);
	assert(_diffuseReflection.size() == 3);
	assert(_specularReflection.size() == 3);
}