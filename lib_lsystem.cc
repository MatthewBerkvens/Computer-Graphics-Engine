#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "lib_lsystem.h"
#include "mylibrary.h"
#include "img_generator.h"
#include "lib3d.h"
#include <assert.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <algorithm>


lib3d::Figure lib_lsystem::generate_3DLSystem(lib3d::Figure& figure, const ini::Configuration &conf, string figurename, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient)
{
	string inputfile = EXTRA_PATH_IF_WINDOWS + conf[figurename]["inputfile"].as_string_or_die();

	LParser::LSystem3D l_system;

	ifstream l3dfilestream(inputfile.c_str());
	assert(l3dfilestream);
	l3dfilestream >> l_system;
	l3dfilestream.close();

	Vector3D H = Vector3D().vector(1, 0, 0);
	Vector3D L = Vector3D().vector(0, 1, 0);
	Vector3D U = Vector3D().vector(0, 0, 1);
	Vector3D pos = Vector3D().point(0, 0, 0);

	double angleInRad = degreesToRad(l_system.get_angle());

	vector<tuple<Vector3D, Vector3D, Vector3D, Vector3D>> stack;

	string initiator = l_system.get_initiator();

	for (unsigned int x = 0; x < initiator.length(); x++)
	{
		LSystem3DIterate(figure, l_system, stack, pos, H, L, U, angleInRad, initiator[x], ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient, 0);
	}

	return figure;
}

void lib_lsystem::LSystem3DIterate(lib3d::Figure& figure, LParser::LSystem3D& l_system, vector<tuple<Vector3D, Vector3D, Vector3D, Vector3D>>& stack, Vector3D& pos, Vector3D& H, Vector3D& L, Vector3D& U, double& angleInRad, char& chr, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient, unsigned int iter)
{
	if (iter < l_system.get_nr_iterations() && l_system.get_alphabet().count(chr))
	{
		string repl = l_system.get_replacement(chr);
		for (unsigned int i = 0; i < repl.length(); i++)
		{
			LSystem3DIterate(figure, l_system, stack, pos, H, L, U, angleInRad, repl[i], ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient, iter + 1);
		}
	}
	else
	{
		if (chr == '+') {
			Vector3D Hnew = H * std::cos(angleInRad) + L * std::sin(angleInRad);
			Vector3D Lnew = -H * std::sin(angleInRad) + L * std::cos(angleInRad);

			H = Hnew;
			L = Lnew;
		}
		else if (chr == '-') {
			Vector3D Hnew = H * std::cos(-angleInRad) + L * std::sin(-angleInRad);
			Vector3D Lnew = -H * std::sin(-angleInRad) + L * std::cos(-angleInRad);

			H = Hnew;
			L = Lnew;
		}
		else if (chr == '^') {
			Vector3D Hnew = H * std::cos(angleInRad) + U * std::sin(angleInRad);
			Vector3D Unew = -H * std::sin(angleInRad) + U * std::cos(angleInRad);

			H = Hnew;
			U = Unew;
		}
		else if (chr == '&') {
			Vector3D Hnew = H * std::cos(-angleInRad) + U * std::sin(-angleInRad);
			Vector3D Unew = -H * std::sin(-angleInRad) + U * std::cos(-angleInRad);

			H = Hnew;
			U = Unew;
		}
		else if (chr == '\\') {
			Vector3D Lnew = L * std::cos(angleInRad) - U * std::sin(angleInRad);
			Vector3D Unew = L * std::sin(angleInRad) + U * std::cos(angleInRad);

			L = Lnew;
			U = Unew;
		}
		else if (chr == '/') {
			Vector3D Lnew = L * std::cos(-angleInRad) - U * std::sin(-angleInRad);
			Vector3D Unew = L * std::sin(-angleInRad) + U * std::cos(-angleInRad);

			L = Lnew;
			U = Unew;
		}
		else if (chr == '|') {
			H = -H;
			L = -L;
		}
		else if (chr == '(') {
			stack.push_back(tuple<Vector3D, Vector3D, Vector3D, Vector3D>(pos, H, L, U));
		}
		else if (chr == ')')
		{
			tuple<Vector3D, Vector3D, Vector3D, Vector3D> popped = stack.back();

			pos = get<0>(popped);
			H = get<1>(popped);
			L = get<2>(popped);
			U = get<3>(popped);

			stack.pop_back();
		}
		else if (l_system.get_alphabet().count(chr))
		{
			Vector3D oldpos = pos;
			pos += H;

			if (l_system.draw(chr))
			{
				unsigned int size = figure.points.size();
				figure.points.push_back(oldpos);
				figure.points.push_back(pos);
				figure.faces.push_back(lib3d::Face({ size, size + 1 }, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient));
			}
		}
	}
}

img::EasyImage lib_lsystem::generate_2DLSystem(const ini::Configuration &conf)
{
	string inputfile = conf["2DLSystem"]["inputfile"].as_string_or_die();
	const unsigned int size = conf["General"]["size"].as_int_or_die();

	lib3d::Color colorBackground(lib3d::colorFromNormalizedDoubleTuple(conf["General"]["backgroundcolor"].as_double_tuple_or_die()));
	lib3d::Color colorLine(lib3d::colorFromNormalizedDoubleTuple(conf["2DLSystem"]["color"].as_double_tuple_or_die()));

	LParser::LSystem2D l_system;

	ifstream l2dfilestream(inputfile.c_str());
	assert(l2dfilestream);
	l2dfilestream >> l_system;
	l2dfilestream.close();

	double current_x = 0;
	double current_y = 0;
	double angle = l_system.get_starting_angle();

	string initiator = l_system.get_initiator();

	vector<lib3d::Line2D> lines;
	vector<lib3d::Point2D> points;
	vector<tuple<double, double, double>> stack;

	for (unsigned int x = 0; x < initiator.length(); x++)
	{
		LSystem2DIterate(lines, points, l_system, stack, current_x, current_y, angle, colorLine, initiator[x], 0);
	}

	return img_generator::imgFrom2DLines(lines, points, size, colorBackground);
}

void lib_lsystem::LSystem2DIterate(vector<lib3d::Line2D>& lines, vector<lib3d::Point2D>& points, LParser::LSystem2D& l_system, vector<tuple<double, double, double>>& stack, double& current_x, double& current_y, double& angle, lib3d::Color& color, char& chr, unsigned int iter)
{
	if (iter < l_system.get_nr_iterations() && l_system.get_alphabet().count(chr))
	{
		string repl = l_system.get_replacement(chr);
		for (unsigned int i = 0; i < repl.length(); i++)
		{
			LSystem2DIterate(lines, points, l_system, stack, current_x, current_y, angle, color, repl[i], iter + 1);
		}
	}
	else
	{
		if (chr == '+') angle = fmod(angle + l_system.get_angle(), 360);
		else if (chr == '-') angle = fmod(angle - l_system.get_angle(), 360);
		else if (chr == '(') stack.push_back(tuple<double, double, double>(current_x, current_y, angle));
		else if (chr == ')')
		{
			tuple<double, double, double> popped = stack.back();

			current_x = get<0>(popped);
			current_y = get<1>(popped);
			angle = get<2>(popped);

			stack.pop_back();
		}
		else if (l_system.get_alphabet().count(chr))
		{
			lib3d::Point2D a = lib3d::Point2D(current_x, current_y);

			current_x += std::cos(degreesToRad(angle));
			current_y += std::sin(degreesToRad(angle));

			lib3d::Point2D b = lib3d::Point2D(current_x, current_y);

			points.push_back(a);
			points.push_back(b);

			if (l_system.draw(chr))
			{
				lines.push_back(lib3d::Line2D(a, b, color));
			}
		}
	}
}