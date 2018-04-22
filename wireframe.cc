//
// Created by matthew.
//

#include "wireframe.h"

img::EasyImage wireframe::generate_Wireframe(const ini::Configuration &conf, bool UseZBuffering)
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

		if (type == "LineDrawing") parseLineDrawing(newFigure, conf, figureName);
		else if (type == "Cube") bodies::createCube(newFigure);
		else if (type == "Tetrahedron") bodies::createTetrahedron(newFigure);
		else if (type == "Octahedron") bodies::createOctahedron(newFigure);
		else if (type == "Icosahedron") bodies::createIcosahedron(newFigure);
		else if (type == "Dodecahedron") bodies::createDodecahedron(newFigure);
		else if (type == "Cone") bodies::createCone(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Cylinder") bodies::createCylinder(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Sphere") bodies::createSphere(newFigure, conf[figureName]["n"].as_int_or_die());
		else if (type == "Torus") bodies::createTorus(newFigure, conf[figureName]["r"].as_double_or_die(), conf[figureName]["R"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die());
		else if (type == "3DLSystem") lib_lsystem::generate_3DLSystem(newFigure, conf, figureName);
		else continue;

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

		std::pair<std::vector<Point2D>, std::vector<Line2D>> pair = lib3d::projectFigures(figures, 1);

		return imgFrom2DLines(pair.second, pair.first, size, backgroundColor, UseZBuffering);
	}
	else return img::EasyImage();
}


void wireframe::parseLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;

	std::vector<double> currentPoint;
	std::vector<int> currentFacePoints;

	for (int i = 0; i < conf[name]["nrPoints"].as_int_or_die(); i++)
	{
		currentPoint = conf[name]["point" + std::to_string(i)].as_double_tuple_or_die();

		Vector3D point = Vector3D().point(currentPoint[0], currentPoint[1], currentPoint[2]);

		points.push_back(point);
	}

	for (int i = 0; i < conf[name]["nrLines"].as_int_or_die(); i++)
	{
		currentFacePoints = conf[name]["line" + std::to_string(i)].as_int_tuple_or_die();

		lib3d::Face newFace;

		newFace.point_indexes.push_back(currentFacePoints[0]);
		newFace.point_indexes.push_back(currentFacePoints[1]);

		faces.push_back(newFace);
	}

	figure.points = points;
	figure.faces = faces;
}