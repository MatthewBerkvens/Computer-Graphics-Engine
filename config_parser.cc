#include "config_parser.h"

void generateFiguresFromConfig(std::vector<Figure>& figures, const ini::Configuration& conf)
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

		Figure newFigure(ambientReflectionVector, diffuseReflectionVector, specularReflectionVector, reflectionCoefficient);

		std::string textureFile;
		if (conf[figureName]["texture"].as_string_if_exists(textureFile))
		{
			std::ifstream ftexture;
			ftexture.open(textureFile.c_str());
			if (!ftexture) ftexture.open((EXTRA_PATH_IF_WINDOWS + textureFile).c_str());
			assert(ftexture);
			ftexture >> newFigure.texture;
			ftexture.close();
		}
		else newFigure.texture = img::EasyImage();

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
			fractals::createFractal(newFigure, figures, conf[figureName]["nrIterations"].as_int_or_die(), conf[figureName]["fractalScale"].as_double_or_die());
		else if (type.length() > 5 && type.substr(0, 5) == "Thick")
			bodies::generateThickFigure(newFigure, figures, conf[figureName]["radius"].as_double_or_die(), conf[figureName]["n"].as_int_or_die(), conf[figureName]["m"].as_int_or_die());
		else if (type != "MengerSponge")
			figures.push_back(newFigure);

		for (std::vector<Figure>::iterator it_figure = std::next(figures.begin(), figureSize); it_figure != figures.end(); it_figure++)
		{
			lib3d::transformFigure(*it_figure, combinedMatrix);

			if (it_figure->texture.get_height() != 0)
			{
				it_figure->textureMethod = conf[figureName]["textureMethod"].as_int_or_die();

				if (it_figure->textureMethod == 0)
				{
					for (std::vector<Face>::iterator it = it_figure->faces.begin(); it != it_figure->faces.end(); it++)
					{
						it_figure->surfaceInformation.push_back(
							{
								it_figure->points[it->point_indexes[0]],
								Vector3D().vector(it_figure->points[it->point_indexes[1]] - it_figure->points[it->point_indexes[0]]),
								Vector3D().vector(it_figure->points[it->point_indexes[3]] - it_figure->points[it->point_indexes[0]]),
							}
						);
					}
				}
				else if (it_figure->textureMethod == 1)
				{
					if (conf[figureName]["p"].exists())
					{
						std::vector<double> p = conf[figureName]["p"].as_double_tuple_or_die();
						std::vector<double> a = conf[figureName]["a"].as_double_tuple_or_die();
						std::vector<double> b = conf[figureName]["b"].as_double_tuple_or_die();

						it_figure->surfaceInformation.push_back(
							{
								Vector3D().point(p[0], p[1], p[2]),
								Vector3D().vector(a[0], a[1], a[2]),
								Vector3D().vector(b[0], b[1], b[2])
							});
					}
					else
					{
						auto minmax_x = std::minmax_element(it_figure->points.begin(), it_figure->points.end(), [&](const Vector3D& a, const Vector3D& b) {
							return a.x < b.x;
						});

						auto minmax_y = std::minmax_element(it_figure->points.begin(), it_figure->points.end(), [&](const Vector3D& a, const Vector3D& b) {
							return a.y < b.y;
						});

						auto minmax_z = std::minmax_element(it_figure->points.begin(), it_figure->points.end(), [&](const Vector3D& a, const Vector3D& b) {
							return a.z < b.z;
						});

						Vector3D p = Vector3D().point(minmax_x.first->x, minmax_y.first->y, minmax_z.first->z);

						Vector3D a = Vector3D().vector(Vector3D().point(minmax_x.first->x, minmax_y.second->y, minmax_z.first->z) - p);

						Vector3D b = Vector3D().vector(Vector3D().point(minmax_x.second->x, minmax_y.first->y, minmax_z.first->z) - p);

						it_figure->surfaceInformation.push_back({ p, a, b });
					}
				}
			}
		}
	}
}


void generateLightsFromConfig(std::vector<Light>& lights, const ini::Configuration& conf)
{
	Vector3D zeroVector = Vector3D().vector(0, 0, 0);

	if (!conf["General"]["nrLights"].exists())
	{
		std::vector<double> ambientLight({ 1, 1, 1 });
		std::vector<double> zeroLight({ 0, 0, 0 });
		lights.push_back(Light(ambientLight, zeroLight, zeroLight, false, false, zeroVector, zeroVector));

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
			Matrix eyePointMatrix = lib3d::eyePointMatrix(Vector3D().point(eye[0], eye[1], eye[2]));
			Matrix lightAsEyeMatrix = lib3d::eyePointMatrix(location);
			location *= eyePointMatrix;
			direction *= eyePointMatrix;

			direction.normalise();

			lights.push_back(Light(ambientLightVector, diffuseLightVector, specularLightVector, true, infinity, direction, location, lightAsEyeMatrix));
		}
		else lights.push_back(Light(ambientLightVector, diffuseLightVector, specularLightVector, false, false, zeroVector, zeroVector));
	}
}

void parse3DLineDrawing(Figure& figure, const ini::Configuration& conf, std::string& name)
{
	std::vector<Vector3D> points;
	std::vector<Face> faces;

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

		Face newFace;

		newFace.point_indexes.push_back(currentFacePoints[0]);
		newFace.point_indexes.push_back(currentFacePoints[1]);

		faces.push_back(newFace);
	}

	figure.points = points;
	figure.faces = faces;
}