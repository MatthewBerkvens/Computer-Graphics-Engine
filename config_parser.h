//
// Created by matthew
//

#ifndef ENGINE_CONFIG_PARSER_H
#define ENGINE_CONFIG_PARSER_H
#include "lib3d.h"
#include "bodies.h"
#include "fractals.h"
#include "ini_configuration.h"
#include "lib_lsystem.h"

namespace config_parser {
	void generateFiguresFromConfig(std::vector<lib3d::Figure>& figures, std::vector<lib3d::Light>& lights, const ini::Configuration& conf);

	void generateLightsFromConfig(std::vector<lib3d::Light>& lights, const ini::Configuration& conf);

	void parseLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient);
}

#endif