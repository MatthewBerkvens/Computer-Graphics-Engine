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
#include "Light.h"

void generateFiguresFromConfig(std::vector<Figure>& figures, const ini::Configuration& conf);

void generateLightsFromConfig(std::vector<Light>& lights, const ini::Configuration& conf);

void parse3DLineDrawing(Figure& figure, const ini::Configuration& conf, std::string& name);

#endif