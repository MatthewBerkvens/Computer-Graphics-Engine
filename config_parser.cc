#include "config_parser.h"

void config_parser::generateFiguresFromConfig(std::vector<lib3d::Figure>& figures, const ini::Configuration& conf)
{
	for (int i = 0; i < conf["General"]["nrFigures"].as_int_or_die(); i++)
	{
		std::string figureName = "Figure" + std::to_string(i);

		std::string type = conf[figureName]["type"].as_string_or_die();

		std::vector<double> ambientReflectionVector;

		if (conf[figureName]["ambientReflection"].exists()) ambientReflectionVector = conf[figureName]["ambientReflection"].as_double_tuple_or_die();
		else ambientReflectionVector = conf[figureName]["color"].as_double_tuple_or_die();

		std::vector<double> diffuseReflectionVector = conf[figureName]["diffuseReflection"].as_double_tuple_or_default({ 0, 0, 0 });
		std::vector<double> specularReflectionVector = conf[figureName]["specularReflection"].as_double_tuple_or_default({ 0, 0, 0 });

		double reflectionCoefficient = conf[figureName]["reflectionCoefficient"].as_double_or_default(0);

		lib3d::Figure newFigure(ambientReflectionVector, diffuseReflectionVector, specularReflectionVector, reflectionCoefficient);

		std::vector<double> center = conf[figureName]["center"].as_double_tuple_or_die();

		Matrix combinedMatrix =
			lib3d::rotateXMatrix(degreesToRad(conf[figureName]["rotateX"].as_double_or_die()))
			* lib3d::rotateYMatrix(degreesToRad(conf[figureName]["rotateY"].as_double_or_die()))
			* lib3d::rotateZMatrix(degreesToRad(conf[figureName]["rotateZ"].as_double_or_die()))
			* lib3d::scaleMatrix(conf[figureName]["scale"].as_double_or_die())
			* lib3d::translateMatrix(Vector3D().vector(center[0], center[1], center[2]));

		unsigned int figureSize = figures.size();

		if (type == "LineDrawing" || type == "ThickLineDrawing") parse3DLineDrawing(newFigure, conf, figureName);
		else if (type == "3DLSystem" || type == "Thick3DLSystem") lib_lsystem::generate_3DLSystem(newFigure, conf, figureName);
		else if (type == "Cube" || type == "ThickCube" || type == "FractalCube") bodies::createCube(newFigure);
		else if (type == "Tetrahedron" || type == "ThickTetrahedron" || type == "FractalTetrahedron") bodies::createTetrahedron(newFigure);
		else if (type == "Octahedron" || type == "ThickOctahedron" || type == "FractalOctahedron") bodies::createOctahedron(newFigure);
		else if (type == "Icosahedron" || type == "ThickIcosahedron" || type == "FractalIcosahedron") bodies::createIcosahedron(newFigure);
		else if (type == "Dodecahedron" || type == "ThickDodecahedron" || type == "FractalDodecahedron") bodies::createDodecahedron(newFigure);
		else if (type == "Cone") bodies::createCone(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Cylinder") bodies::createCylinder(newFigure, conf[figureName]["n"].as_int_or_die(), conf[figureName]["height"].as_double_or_die());
		else if (type == "Sphere") bodies::createSphere(newFigure, conf[figureName]["n"].as_int_or_die());
		else if (type == "Torus") bodies::createTorus(newFigure, conf[figureName]["r"].as_double_or_die(), conf[figureName]["R"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die());
		else if (type == "BuckyBall" || type == "ThickBuckyBall" || type == "FractalBuckyBall") bodies::createBuckyBall(newFigure);
		else if (type == "MengerSponge")
		{
			bodies::createCube(newFigure);

			fractals::createMengerSponge(newFigure, figures, conf[figureName]["nrIterations"].as_int_or_die());
		}


		if (type.length() > 7 && type.substr(0, 7) == "Fractal")
		{
			fractals::createFractal(newFigure, figures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());

			for (std::vector<lib3d::Figure>::iterator it_figure = std::next(figures.begin(), figureSize); it_figure != figures.end(); it_figure++)
			{
				lib3d::transformFigure(*it_figure, combinedMatrix);
			}
		}
		else if (type.length() > 5 && type.substr(0, 5) == "Thick")
		{
			bodies::generateThickFigure(newFigure, figures, conf[figureName]["radius"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die());

			for (std::vector<lib3d::Figure>::iterator it_figure = std::next(figures.begin(), figureSize); it_figure != figures.end(); it_figure++)
			{
				lib3d::transformFigure(*it_figure, combinedMatrix);
			}

			continue;
		}
		else
		{
			figures.push_back(newFigure);

			for (std::vector<lib3d::Figure>::iterator it_figure = std::next(figures.begin(), figureSize); it_figure != figures.end(); it_figure++)
			{
				lib3d::transformFigure(*it_figure, combinedMatrix);
			}
		}
	}
}


void config_parser::generateLightsFromConfig(std::vector<lib3d::Light>& lights, const ini::Configuration& conf)
{
	Vector3D zeroVector = Vector3D().vector(0, 0, 0);

	if (!conf["General"]["nrLights"].exists())
	{
		std::vector<double> ambientLight({ 1, 1, 1 });
		std::vector<double> zeroLight({ 0, 0, 0 });
		lights.push_back(lib3d::Light(ambientLight, zeroLight, zeroLight, false, false, zeroVector, zeroVector));

		return;
	}

	for (int i = 0; i < conf["General"]["nrLights"].as_int_or_die(); i++)
	{
		std::string lightName = "Light" + std::to_string(i);

		std::vector<double> ambientLightVector = conf[lightName]["ambientLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));
		std::vector<double> diffuseLightVector = conf[lightName]["diffuseLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));
		std::vector<double> specularLightVector = conf[lightName]["specularLight"].as_double_tuple_or_default(ini::DoubleTuple({ 0,0,0 }));

		bool infinity;
		if (conf[lightName]["infinity"].as_bool_if_exists(infinity))
		{
			std::vector<double> directionDouble = conf[lightName]["direction"].as_double_tuple_or_default({ 0,0,0 });
			std::vector<double> locationDouble = conf[lightName]["location"].as_double_tuple_or_default({ 0,0,0 });

			Vector3D direction = Vector3D().vector(directionDouble[0], directionDouble[1], directionDouble[2]);
			Vector3D location = Vector3D().point(locationDouble[0], locationDouble[1], locationDouble[2]);

			std::vector<double> eye = conf["General"]["eye"].as_double_tuple_or_die();
			Matrix eyePointMatrix = lib3d::transformEyePointMatrix(Vector3D().point(eye[0], eye[1], eye[2]));
			Matrix lightAsEyeMatrix = lib3d::transformEyePointMatrix(location);
			location *= eyePointMatrix;
			direction *= eyePointMatrix;

			direction.normalise();

			lights.push_back(lib3d::Light(ambientLightVector, diffuseLightVector, specularLightVector, true, infinity, direction, location, lightAsEyeMatrix));
		}
		else lights.push_back(lib3d::Light(ambientLightVector, diffuseLightVector, specularLightVector, false, false, zeroVector, zeroVector));
	}
}

void config_parser::parse3DLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name)
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
