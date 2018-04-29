#include "config_parser.h"

void config_parser::generateFiguresFromConfig(std::vector<lib3d::Figure>& figures, std::vector<lib3d::Light>& lights, const ini::Configuration& conf)
{
	for (int i = 0; i < conf["General"]["nrFigures"].as_int_or_die(); i++)
	{
		std::string figureName = "Figure" + std::to_string(i);

		std::string type = conf[figureName]["type"].as_string_or_die();

		lib3d::Figure newFigure;


		lib3d::Color ambientReflection;

		if (conf[figureName]["ambientReflection"].exists()) ambientReflection = lib3d::colorFromNormalizedDoubleTuple(conf[figureName]["ambientReflection"].as_double_tuple_or_die());
		else ambientReflection = lib3d::colorFromNormalizedDoubleTuple(conf[figureName]["color"].as_double_tuple_or_die());

		lib3d::Color diffuseReflection = lib3d::colorFromNormalizedDoubleTuple(conf[figureName]["diffuseReflection"].as_double_tuple_or_default({ 0, 0, 0 }));
		lib3d::Color specularReflection = lib3d::colorFromNormalizedDoubleTuple(conf[figureName]["specularReflection"].as_double_tuple_or_default({ 0, 0, 0 }));
		double reflectionCoefficient = conf[figureName]["reflectionCoefficient"].as_double_or_default(0);


		std::vector<double> center = conf[figureName]["center"].as_double_tuple_or_die();

		Matrix combinedMatrix =
			lib3d::rotateXMatrix(degreesToRad(conf[figureName]["rotateX"].as_double_or_die()))
			* lib3d::rotateYMatrix(degreesToRad(conf[figureName]["rotateY"].as_double_or_die()))
			* lib3d::rotateZMatrix(degreesToRad(conf[figureName]["rotateZ"].as_double_or_die()))
			* lib3d::scaleMatrix(conf[figureName]["scale"].as_double_or_die())
			* lib3d::translateMatrix(Vector3D().vector(center[0], center[1], center[2]));

		if (type == "LineDrawing") parseLineDrawing(newFigure, conf, figureName, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "3DLSystem") lib_lsystem::generate_3DLSystem(newFigure, conf, figureName, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Cube") bodies::createCube(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Tetrahedron") bodies::createTetrahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Octahedron") bodies::createOctahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Icosahedron") bodies::createIcosahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Dodecahedron") bodies::createDodecahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Cone") bodies::createCone(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die(), ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Cylinder") bodies::createCylinder(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die(), ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Sphere") bodies::createSphere(newFigure, conf[figureName]["n"].as_int_or_die(), ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "Torus") bodies::createTorus(newFigure, conf[figureName]["r"].as_double_or_die(), conf[figureName]["R"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die(), ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else if (type == "BuckyBall") bodies::createBuckyBall(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);
		else
		{
			std::vector<lib3d::Figure> newFigures;

			if (type == "FractalCube")
			{
				bodies::createCube(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "FractalDodecahedron")
			{
				bodies::createDodecahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "FractalIcosahedron")
			{
				bodies::createIcosahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "FractalOctahedron")
			{
				bodies::createOctahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "FractalTetrahedron")
			{
				bodies::createTetrahedron(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "FractalBuckyBall")
			{
				bodies::createBuckyBall(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createFractal(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
			}
			else if (type == "MengerSponge")
			{
				bodies::createCube(newFigure, ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

				fractals::createMengerSponge(newFigure, newFigures, conf[figureName]["nrIterations"].as_int_or_die());
			}
			else continue;

			for (std::vector<lib3d::Figure>::iterator it_figure = newFigures.begin(); it_figure != newFigures.end(); ++it_figure)
			{
				for (std::vector<Vector3D>::iterator it_point = it_figure->points.begin(); it_point != it_figure->points.end(); ++it_point)
				{
					(*it_point) *= combinedMatrix;
				}

				figures.push_back(*it_figure);
			}

			continue;
		}

		for (std::vector<Vector3D>::iterator it = newFigure.points.begin(); it != newFigure.points.end(); ++it)
		{
			(*it) *= combinedMatrix;
		}

		figures.push_back(newFigure);
	}

	std::vector<double> eye = conf["General"]["eye"].as_double_tuple_or_die();

	lib3d::applyEyeTransform(figures, Vector3D().point(eye[0], eye[1], eye[2]));
}


void config_parser::generateLightsFromConfig(std::vector<lib3d::Light>& lights, const ini::Configuration& conf)
{
	Vector3D zeroVector = Vector3D().vector(0, 0, 0);

	if (!conf["General"]["nrLights"].exists())
	{
		std::tuple<double, double, double> ambientLight(255, 255, 255);
		std::tuple<double, double, double> zeroLight(0, 0, 0);
		lights.push_back(lib3d::Light(ambientLight, zeroLight, zeroLight, false, false, zeroVector, zeroVector));

		return;
	}

	for (int i = 0; i < conf["General"]["nrLights"].as_int_or_die(); i++)
	{
		std::string lightName = "Light" + std::to_string(i);

		std::vector<double> ambientLightVector = conf[lightName]["ambientLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));
		std::vector<double> diffuseLightVector = conf[lightName]["diffuseLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));
		std::vector<double> specularLightVector = conf[lightName]["specularLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));

		std::tuple<double, double, double> ambientLight(ambientLightVector[0], ambientLightVector[1], ambientLightVector[2]);
		std::tuple<double, double, double> diffuseLight(diffuseLightVector[0], diffuseLightVector[1], diffuseLightVector[2]);
		std::tuple<double, double, double> specularLight(specularLightVector[0], specularLightVector[1], specularLightVector[2]);

		bool infinity;
		if (conf[lightName]["infinity"].as_bool_if_exists(infinity))
		{
			std::vector<double> directionDouble = conf[lightName]["direction"].as_double_tuple_or_default({ 0,0,0 });
			std::vector<double> locationDouble = conf[lightName]["location"].as_double_tuple_or_default({ 0,0,0 });

			Vector3D direction = Vector3D().vector(directionDouble[0], directionDouble[1], directionDouble[2]);
			Vector3D location = Vector3D().point(locationDouble[0], locationDouble[1], locationDouble[2]);

			std::vector<double> eye = conf["General"]["eye"].as_double_tuple_or_die();
			Matrix eyePointMatrix = lib3d::transformEyePointMatrix(Vector3D().point(eye[0], eye[1], eye[2]));
			location *= eyePointMatrix;
			direction *= eyePointMatrix;

			direction.normalise();

			lights.push_back(lib3d::Light(ambientLight, diffuseLight, specularLight, true, infinity, direction, location));
		}
		else lights.push_back(lib3d::Light(ambientLight, diffuseLight, specularLight, false, false, zeroVector, zeroVector));
	}
}

void config_parser::parseLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name, lib3d::Color& ambientReflection, lib3d::Color& diffuseReflection, lib3d::Color& specularReflection, double reflectionCoefficient)
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

		lib3d::Face newFace(ambientReflection, diffuseReflection, specularReflection, reflectionCoefficient);

		newFace.point_indexes.push_back(currentFacePoints[0]);
		newFace.point_indexes.push_back(currentFacePoints[1]);

		faces.push_back(newFace);
	}

	figure.points = points;
	figure.faces = faces;
}
