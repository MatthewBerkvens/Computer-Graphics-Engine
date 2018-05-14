#include "Figure.h"

Figure::Figure(std::vector<double>& _ambientReflection, std::vector<double>& _diffuseReflection, std::vector<double>& _specularReflection, double _reflectionCoefficient)
	: ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient)
{
	assert(_ambientReflection.size() == 3);
	assert(_diffuseReflection.size() == 3);
	assert(_specularReflection.size() == 3);
}