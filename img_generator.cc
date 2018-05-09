//
// Created by matthew
//

#include "img_generator.h"

img::EasyImage img_generator::imgFrom2DLines(std::vector<lib3d::Line2D>& lines, std::vector<lib3d::Point2D>& points, const double size, lib3d::Color& backgroundColor)
{
	auto minmax_x = std::minmax_element(points.begin(), points.end(), [](const lib3d::Point2D &a, const lib3d::Point2D &b) {
		return a.x < b.x;
	});
	auto minmax_y = std::minmax_element(points.begin(), points.end(), [](const lib3d::Point2D &a, const lib3d::Point2D &b) {
		return a.y < b.y;
	});

	double min_x = minmax_x.first->x;
	double max_x = minmax_x.second->x;

	double min_y = minmax_y.first->y;
	double max_y = minmax_y.second->y;


	double range_x = max_x - min_x;
	double range_y = max_y - min_y;

	double image_x = ((double)size) * range_x / std::max(range_x, range_y);
	double image_y = ((double)size) * range_y / std::max(range_x, range_y);

	double d = 0.95 * (image_x / range_x);

	double offset_x = (image_x / 2.0) - (d*(min_x + max_x) / 2.0);
	double offset_y = (image_y / 2.0) - (d*(min_y + max_y) / 2.0);

	img::EasyImage image = img::EasyImage(roundToInt(image_x), roundToInt(image_y), backgroundColor);

	for (std::vector<lib3d::Line2D>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		image.draw_line(
			roundToInt((d * it->a.x) + offset_x),
			roundToInt((d * it->a.y) + offset_y),
			roundToInt((d * it->b.x) + offset_x),
			roundToInt((d * it->b.y) + offset_y),
			it->color);
	}

	return image;
}

img::EasyImage img_generator::imgFromFigures_Wireframe(std::vector<lib3d::Figure>& figures, const double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor)
{
	std::tuple<std::pair<int, int>, double, std::pair<double, double>> imageFacts = getImageFacts(figures, size, eyeMatrix);
	std::pair<int, int> imageSize = std::get<0>(imageFacts);
	double imageD = std::get<1>(imageFacts);
	std::pair<double, double> imageOffsets = std::get<2>(imageFacts);

	img::EasyImage image(imageSize.first, imageSize.second, backgroundColor);
	lib3d::ZBuffer zbuffer(imageSize.first, imageSize.second);

	for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
	{
		lib3d::transformFigure(*it_figure, eyeMatrix);
	}

	std::pair<std::vector<lib3d::Point2D>, std::vector<lib3d::Line2D>> pair = lib3d::projectFigures(figures, 1);

	for (std::vector<lib3d::Line2D>::iterator it = pair.second.begin(); it != pair.second.end(); ++it)
	{
		image.draw_line(
			roundToInt((imageD * it->a.x) + imageOffsets.first),
			roundToInt((imageD * it->a.y) + imageOffsets.second),
			roundToInt((imageD * it->b.x) + imageOffsets.first),
			roundToInt((imageD * it->b.y) + imageOffsets.second),
			it->color);
	}

	return image;
}

img::EasyImage img_generator::imgFromFigures_ZBufferWireframe(std::vector<lib3d::Figure>& figures, const double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor)
{
	std::tuple<std::pair<int, int>, double, std::pair<double, double>> imageFacts = getImageFacts(figures, size, eyeMatrix);
	std::pair<int, int> imageSize = std::get<0>(imageFacts);
	double imageD = std::get<1>(imageFacts);
	std::pair<double, double> imageOffsets = std::get<2>(imageFacts);

	img::EasyImage image(imageSize.first, imageSize.second, backgroundColor);
	lib3d::ZBuffer zbuffer(imageSize.first, imageSize.second);

	for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
	{
		lib3d::transformFigure(*it_figure, eyeMatrix);
	}

	std::pair<std::vector<lib3d::Point2D>, std::vector<lib3d::Line2D>> pair = lib3d::projectFigures(figures, 1);

	for (std::vector<lib3d::Line2D>::iterator it = pair.second.begin(); it != pair.second.end(); ++it)
	{
		image.draw_zbuf_line(
			zbuffer,
			roundToInt((imageD * it->a.x) + imageOffsets.first),
			roundToInt((imageD * it->a.y) + imageOffsets.second),
			it->z_a,
			roundToInt((imageD * it->b.x) + imageOffsets.first),
			roundToInt((imageD * it->b.y) + imageOffsets.second),
			it->z_b,
			it->color);
	}

	return image;
}

img::EasyImage img_generator::imgFromFigures_Triangles(std::vector<lib3d::Figure>& figures, double size, Matrix& eyeMatrix, lib3d::Color& backgroundColor, std::vector<lib3d::Light>& lights, bool shadow, unsigned int shadowSize)
{
	std::tuple<std::pair<int, int>, double, std::pair<double, double>> imageFacts = getImageFacts(figures, size, eyeMatrix);
	std::pair<int, int> imageSize = std::get<0>(imageFacts);
	double imageD = std::get<1>(imageFacts);
	std::pair<double, double> imageOffsets = std::get<2>(imageFacts);

	img::EasyImage image(imageSize.first, imageSize.second, backgroundColor);
	lib3d::ZBuffer zbuffer(imageSize.first, imageSize.second);

	Matrix invertedEyeMatrix(eyeMatrix);
	invertedEyeMatrix.inv();

	if (shadow)
	{
		for (std::vector<lib3d::Light>::iterator it_light = lights.begin(); it_light != lights.end(); it_light++)
		{
			std::tuple<std::pair<int, int>, double, std::pair<double, double>> shadowFacts = getImageFacts(figures, shadowSize, it_light->lightAsEyeMatrix);
			std::pair<int, int> shadowSize = std::get<0>(shadowFacts);
			double shadowD = std::get<1>(shadowFacts);
			std::pair<double, double> shadowOffsets = std::get<2>(shadowFacts);

			it_light->shadowmask = lib3d::ZBuffer(shadowSize.first, shadowSize.second);
			it_light->d = shadowD;
			it_light->dx = shadowOffsets.first;
			it_light->dy = shadowOffsets.second;

			img::EasyImage maskimage(shadowSize.first, shadowSize.second, backgroundColor);

			for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
			{
				for (std::vector<lib3d::Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); ++it_face)
				{
					for (std::vector<unsigned int>::iterator it_face_pt_index = std::next(it_face->point_indexes.begin()); it_face_pt_index != std::prev(it_face->point_indexes.end()); it_face_pt_index++)
					{
						/*it_light->addShadowTriangle(
							it_figure->points[*it_face->point_indexes.begin()] * it_light->lightAsEyeMatrix,
							it_figure->points[*it_face_pt_index] * it_light->lightAsEyeMatrix,
							it_figure->points[*std::next(it_face_pt_index)] * it_light->lightAsEyeMatrix);*/
						Vector3D A = it_figure->points[*it_face->point_indexes.begin()] * it_light->lightAsEyeMatrix;
						Vector3D B = it_figure->points[*it_face_pt_index] * it_light->lightAsEyeMatrix;
						Vector3D C = it_figure->points[*std::next(it_face_pt_index)] * it_light->lightAsEyeMatrix;
						lib3d::Point2D projected_A = lib3d::Point2D(((it_light->d * A.x) / -A.z) + it_light->dx, ((it_light->d * A.y) / -A.z) + it_light->dy);
						lib3d::Point2D projected_B = lib3d::Point2D(((it_light->d * B.x) / -B.z) + it_light->dx, ((it_light->d * B.y) / -B.z) + it_light->dy);
						lib3d::Point2D projected_C = lib3d::Point2D(((it_light->d * C.x) / -C.z) + it_light->dx, ((it_light->d * C.y) / -C.z) + it_light->dy);

						Vector3D grav = Vector3D().point(
							((projected_A.x + projected_B.x + projected_C.x) / 3), //xg
							((projected_A.y + projected_B.y + projected_C.y) / 3), //yg
							1 / (3 * A.z) + 1 / (3 * B.z) + 1 / (3 * C.z) // 1/zg
						);

						unsigned int y_min = roundToInt(std::min(projected_A.y, std::min(projected_B.y, projected_C.y)) + 0.5);
						unsigned int y_max = roundToInt(std::max(projected_A.y, std::max(projected_B.y, projected_C.y)) - 0.5);

						double w1 = ((B.y - A.y) * (C.z - A.z)) - ((B.z - A.z) * (C.y - A.y));

						double w2 = ((B.z - A.z) * (C.x - A.x)) - ((B.x - A.x) * (C.z - A.z));

						double w3 = ((B.x - A.x) * (C.y - A.y)) - ((B.y - A.y) * (C.x - A.x));

						double k = w1 * A.x + w2 * A.y + w3 * A.z;

						double dzdx = w1 / (-it_light->d * k);
						double dzdy = w2 / (-it_light->d * k);

						for (unsigned int y_cur = y_min; y_cur <= y_max; y_cur++)
						{
							double x_min = std::numeric_limits<double>::infinity();
							double x_max = -std::numeric_limits<double>::infinity();

							for (unsigned int i = 0; i < 3; i++)
							{
								const lib3d::Point2D& P = i < 2 ? (i < 1 ? projected_A : projected_B) : projected_C;
								const lib3d::Point2D& Q = (i + 1) % 3 < 2 ? ((i + 1) % 3 < 1 ? projected_A : projected_B) : projected_C;

								if (P.y == Q.y || (((double)y_cur - P.y) * ((double)y_cur - Q.y)) > 0) continue;

								x_min = std::min(x_min, Q.x + ((P.x - Q.x) * (((double)y_cur - Q.y) / (P.y - Q.y))));
								x_max = std::max(x_max, Q.x + ((P.x - Q.x) * (((double)y_cur - Q.y) / (P.y - Q.y))));
							}

							for (int x_cur = roundToInt(x_min + 0.5); x_cur <= roundToInt(x_max - 0.5); x_cur++)
							{
								double z_inv =
									(grav.z) + // 1/zg
									((x_cur - grav.x) * dzdx) + // (x - xg) * dzdx
									((y_cur - grav.y) * dzdy); // (y - yg) * dzdy

								if (z_inv < it_light->shadowmask[x_cur][y_cur])
								{
									maskimage(x_cur, y_cur) = lib3d::Color(255, 255, 255);
									it_light->shadowmask[x_cur][y_cur] = z_inv;
								}
							}
						}
					}
				}
			}
		}
	}


	for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
	{
		for (std::vector<lib3d::Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); ++it_face)
		{
			for (std::vector<unsigned int>::iterator it_face_pt_index = std::next(it_face->point_indexes.begin()); it_face_pt_index != std::prev(it_face->point_indexes.end()); it_face_pt_index++)
			{
				image.draw_zbuf_triag(
					zbuffer,
					it_figure->points[*it_face->point_indexes.begin()] * eyeMatrix,
					it_figure->points[*it_face_pt_index] * eyeMatrix,
					it_figure->points[*std::next(it_face_pt_index)] * eyeMatrix,
					imageD,
					imageOffsets.first,
					imageOffsets.second,
					it_figure->ambientReflection,
					it_figure->diffuseReflection,
					it_figure->specularReflection,
					it_figure->reflectionCoefficient,
					lights,
					shadow,
					invertedEyeMatrix);
			}
		}
	}

	return image;
}


std::tuple<std::pair<int, int>, double, std::pair<double, double>> img_generator::getImageFacts(std::vector<lib3d::Figure>& figures, double size, Matrix& eyeMatrix)
{
	double min_x = 0;
	double max_x = 0;

	double min_y = 0;
	double max_y = 0;

	for (std::vector<lib3d::Figure>::iterator it = figures.begin(); it != figures.end(); it++)
	{
		auto minmax_x = std::minmax_element(it->points.begin(), it->points.end(), [&](const Vector3D& a, const Vector3D& b) {
			return lib3d::projectPoint(a * eyeMatrix, 1).x < lib3d::projectPoint(b * eyeMatrix, 1).x;
		});
		auto minmax_y = std::minmax_element(it->points.begin(), it->points.end(), [&](const Vector3D& a, const Vector3D& b) {
			return lib3d::projectPoint(a * eyeMatrix, 1).y < lib3d::projectPoint(b * eyeMatrix, 1).y;
		});

		min_x = std::min(min_x, lib3d::projectPoint(*minmax_x.first * eyeMatrix, 1).x);
		max_x = std::max(max_x, lib3d::projectPoint(*minmax_x.second * eyeMatrix, 1).x);

		min_y = std::min(min_y, lib3d::projectPoint(*minmax_y.first * eyeMatrix, 1).y);
		max_y = std::max(max_y, lib3d::projectPoint(*minmax_y.second * eyeMatrix, 1).y);
	}

	double range_x = max_x - min_x;
	double range_y = max_y - min_y;

	double image_x = size * range_x / std::max(range_x, range_y);
	double image_y = size * range_y / std::max(range_x, range_y);

	double d = 0.95 * (image_x / range_x);

	double dx = (image_x / 2.0) - (d * (min_x + max_x) / 2.0);
	double dy = (image_y / 2.0) - (d * (min_y + max_y) / 2.0);

	return std::tuple<std::pair<int, int>, double, std::pair<double, double>>(std::pair<int, int>(roundToInt(image_x), roundToInt(image_y)), d, std::pair<double, double>(dx, dy));
}