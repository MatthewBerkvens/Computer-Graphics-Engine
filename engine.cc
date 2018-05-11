#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "lib_lsystem.h"
#include "lib3d.h"
#include "config_parser.h"
#include "img_generator.h"
#include "Projecting.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

img::EasyImage generate_image(const ini::Configuration &configuration)
{
	std::string type = configuration["General"]["type"].as_string_or_die();
	double size = configuration["General"]["size"].as_double_or_die();

	if (type == "2DLSystem") return lib_lsystem::generate_2DLSystem(configuration);

	std::vector<Light> lights;
	generateLightsFromConfig(lights, configuration);

	std::vector<Figure> figures;
	generateFiguresFromConfig(figures, configuration);

	Color backgroundColor = colorFromNormalizedDoubleTuple(configuration["General"]["backgroundcolor"].as_double_tuple_or_die());

	std::vector<double> eye = configuration["General"]["eye"].as_double_tuple_or_die();
	Matrix eyeMatrix = lib3d::eyePointMatrix(Vector3D().point(eye[0], eye[1], eye[2]));

	if (type == "Wireframe")
		return img_generator::imgFromFigures_Wireframe(figures, size, eyeMatrix, backgroundColor);
	else if (type == "ZBufferedWireframe")
		return img_generator::imgFromFigures_ZBufferWireframe(figures, size, eyeMatrix, backgroundColor);
	else if (type == "ZBuffering")
		return img_generator::imgFromFigures_Triangles(figures, size, eyeMatrix, backgroundColor, lights);
	else if (type == "LightedZBuffering")
		return img_generator::imgFromFigures_Triangles(figures, size, eyeMatrix, backgroundColor, lights, configuration["General"]["shadowEnabled"].as_bool_or_default(false), configuration["General"]["shadowMask"].as_int_or_default(0));

	else return img::EasyImage();
}

int main(int argc, char const* argv[])
{
	int retVal = 0;
	try
	{
		for (int i = 1; i < argc; ++i)
		{
			ini::Configuration conf;
			try
			{
				std::ifstream fin(argv[i]);
				if (!fin) continue;
				fin >> conf;
				fin.close();
			}
			catch (ini::ParseException& ex)
			{
				std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
				retVal = 1;
				continue;
			}

			img::EasyImage image = generate_image(conf);
			if (image.get_height() > 0 && image.get_width() > 0)
			{
				std::string fileName(argv[i]);
				std::string::size_type pos = fileName.rfind('.');
				if (pos == std::string::npos)
				{
					//filename does not contain a '.' --> append a '.bmp' suffix
					fileName += ".bmp";
				}
				else
				{
					fileName = fileName.substr(0, pos) + ".bmp";
				}
				try
				{
					std::ofstream f_out(fileName.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
					f_out << image;

				}
				catch (std::exception& ex)
				{
					std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
					retVal = 1;
				}
			}
			else
			{
				std::cout << "Could not generate image for " << argv[i] << std::endl;
			}
		}
	}
	catch (const std::bad_alloc &exception)
	{
		(void)exception; //pesky unused variable warnings
		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
		//(Unless of course you are already consuming the maximum allowed amount of memory)
		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
		std::cerr << "Error: insufficient memory" << std::endl;
		retVal = 100;
	}
	return retVal;
}
