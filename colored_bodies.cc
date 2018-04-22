//
// Created by matthew
//

#include "colored_bodies.h"
#include <assert.h>
#include <algorithm>

img::EasyImage colored_bodies::generate_ColoredBodies(const ini::Configuration &conf)
{
	double size = conf["General"]["size"].as_double_or_die();

	img::Color backgroundColor = colorFromNormalizedDoubleTuple(conf["General"]["backgroundcolor"].as_double_tuple_or_die());

	std::vector<lib3d::Figure> figures;

	for (int i = 0; i < conf["General"]["nrFigures"].as_int_or_die(); i++)
	{
		std::string figureName = "Figure" + std::to_string(i);

		std::string type = conf[figureName]["type"].as_string_or_die();

		lib3d::Figure newFigure;
		newFigure.color = colorFromNormalizedDoubleTuple(conf[figureName]["color"].as_double_tuple_or_die());

		if (type == "Cube") bodies::createCube(newFigure);
		else if (type == "Tetrahedron") bodies::createTetrahedron(newFigure);
		else if (type == "Octahedron") bodies::createOctahedron(newFigure);
		else if (type == "Icosahedron") bodies::createIcosahedron(newFigure);
		else if (type == "Dodecahedron") bodies::createDodecahedron(newFigure);
		else if (type == "Cone") bodies::createCone(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Cylinder") bodies::createCylinder(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Sphere") bodies::createSphere(newFigure, conf[figureName]["n"].as_int_or_die());
		else if (type == "Torus") bodies::createTorus(newFigure, conf[figureName]["r"].as_double_or_die(), conf[figureName]["R"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die());

		std::vector<lib3d::Face> triangulatedFaces;
		for (std::vector<lib3d::Face>::iterator it = newFigure.faces.begin(); it != newFigure.faces.end(); it++)
		{
			std::vector<lib3d::Face> newFaces;
			triangulate(newFaces, *it);
			triangulatedFaces.insert(triangulatedFaces.end(), newFaces.begin(), newFaces.end());
		}
		newFigure.faces = triangulatedFaces;

		std::vector<double> center = conf[figureName]["center"].as_double_tuple_or_die();

		Matrix combinedMatrix =
			lib3d::rotateXMatrix(degreesToRad(conf[figureName]["rotateX"].as_double_or_die()))
			* lib3d::rotateYMatrix(degreesToRad(conf[figureName]["rotateY"].as_double_or_die()))
			* lib3d::rotateZMatrix(degreesToRad(conf[figureName]["rotateZ"].as_double_or_die()))
			* lib3d::scaleMatrix(conf[figureName]["scale"].as_double_or_die())
			* lib3d::translateMatrix(Vector3D().vector(center[0], center[1], center[2]));

		for (std::vector<Vector3D>::iterator it = newFigure.points.begin(); it != newFigure.points.end(); ++it)
		{
			(*it) *= combinedMatrix;
		}

		figures.push_back(newFigure);
	}

	std::vector<double> eye = conf["General"]["eye"].as_double_tuple_or_die();
	if (figures.size() > 0)
	{
		lib3d::applyEyeTransform(figures, Vector3D().point(eye[0], eye[1], eye[2]));

		return imgFromTriangleFigures(figures, size, backgroundColor);
	}
	return img::EasyImage();
}

void colored_bodies::triangulate(std::vector<lib3d::Face>& out, const lib3d::Face& in)
{
	if (in.point_indexes.size() <= 3) out.push_back(in);
	for (size_t i = 1; i < in.point_indexes.size() - 1; i++)
	{
		out.push_back(lib3d::Face().point_indexes = { in.point_indexes[0], in.point_indexes[i], in.point_indexes[i + 1] });
	}
}

img::EasyImage colored_bodies::imgFromTriangleFigures(std::vector<lib3d::Figure>& figures, double size, img::Color& backgroundColor)
{
	double min_x = 0;
	double max_x = 0;

	double min_y = 0;
	double max_y = 0;

	for (std::vector<lib3d::Figure>::iterator it = figures.begin(); it != figures.end(); it++)
	{
		auto minmax_x = std::minmax_element(it->points.begin(), it->points.end(), [](const Vector3D& a, const Vector3D& b) {
			return a.x < b.x;
		});
		auto minmax_y = std::minmax_element(it->points.begin(), it->points.end(), [](const Vector3D& a, const Vector3D& b) {
			return a.y < b.y;
		});

		min_x = std::min(min_x, lib3d::projectPoint(*minmax_x.first, 1).x);
		max_x = std::max(max_x, lib3d::projectPoint(*minmax_x.second, 1).x);

		min_y = std::min(min_y, lib3d::projectPoint(*minmax_y.first, 1).y);
		max_y = std::max(max_y, lib3d::projectPoint(*minmax_y.second, 1).y);
	}

	double range_x = max_x - min_x;
	double range_y = max_y - min_y;

	double image_x = size * range_x / std::max(range_x, range_y);
	double image_y = size * range_y / std::max(range_x, range_y);

	double d = 0.95 * (image_x / range_x);

	img::EasyImage image = img::EasyImage(roundToInt(image_x), roundToInt(image_y), backgroundColor);

	double offset_x = (image_x / 2.0) - (d*(min_x + max_x) / 2.0);
	double offset_y = (image_y / 2.0) - (d*(min_y + max_y) / 2.0);

	img::ZBuffer zbuffer = img::ZBuffer(roundToInt(image_x), roundToInt(image_y));
	for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
	{
		for (std::vector<lib3d::Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); ++it_face)
		{
			assert(it_face->point_indexes.size() == 3);
			image.draw_zbuf_triag(
				zbuffer,
				it_figure->points[it_face->point_indexes[0]],
				it_figure->points[it_face->point_indexes[1]],
				it_figure->points[it_face->point_indexes[2]],
				d,
				offset_x,
				offset_y,
				it_figure->color);
		}
	}

	return image;
}