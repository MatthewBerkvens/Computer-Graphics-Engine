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

img::EasyImage img_generator::imgFromZBuffered2DLines(std::vector<lib3d::Line2D>& lines, std::vector<lib3d::Point2D>& points, const double size, lib3d::Color& backgroundColor)
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

	img::EasyImage image(roundToInt(image_x), roundToInt(image_y), backgroundColor);
	lib3d::ZBuffer zbuffer(roundToInt(image_x), roundToInt(image_y));

	for (std::vector<lib3d::Line2D>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		image.draw_zbuf_line(
			zbuffer,
			roundToInt((d * it->a.x) + offset_x),
			roundToInt((d * it->a.y) + offset_y),
			it->z_a,
			roundToInt((d * it->b.x) + offset_x),
			roundToInt((d * it->b.y) + offset_y),
			it->z_b,
			it->color);
	}

	return image;
}

img::EasyImage img_generator::imgFromTriangleFigures(std::vector<lib3d::Figure>& figures, double size, lib3d::Color& backgroundColor, std::vector<lib3d::Light>& lights, bool shadow, unsigned int shadowSize, Matrix eyeMatrix)
{
	double min_x = 0;
	double max_x = 0;

	double min_y = 0;
	double max_y = 0;

	for (std::vector<lib3d::Figure>::iterator it = figures.begin(); it != figures.end(); it++)
	{
		auto minmax_x = std::minmax_element(it->points.begin(), it->points.end(), [](const Vector3D& a, const Vector3D& b) {
			return lib3d::projectPoint(a, 1).x < lib3d::projectPoint(b, 1).x;
		});
		auto minmax_y = std::minmax_element(it->points.begin(), it->points.end(), [](const Vector3D& a, const Vector3D& b) {
			return lib3d::projectPoint(a, 1).y < lib3d::projectPoint(b, 1).y;
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

	img::EasyImage image(roundToInt(image_x), roundToInt(image_y), backgroundColor);
	lib3d::ZBuffer zbuffer(roundToInt(image_x), roundToInt(image_y));

	double offset_x = (image_x / 2.0) - (d * (min_x + max_x) / 2.0);
	double offset_y = (image_y / 2.0) - (d * (min_y + max_y) / 2.0);

	if (shadow)
	{
		eyeMatrix.inv();

		for (std::vector<lib3d::Light>::iterator it_light = lights.begin(); it_light != lights.end(); it_light++)
		{
			double min_x_shadow = 0;
			double max_x_shadow = 0;

			double min_y_shadow = 0;
			double max_y_shadow = 0;

			for (std::vector<lib3d::Figure>::iterator it = figures.begin(); it != figures.end(); it++)
			{
				auto minmax_x_shadow = std::minmax_element(it->points.begin(), it->points.end(), [&](const Vector3D& a, const Vector3D& b) {
					return lib3d::projectPoint(((a * eyeMatrix) * it_light->lightAsEyeMatrix), 1).x < lib3d::projectPoint(((b * eyeMatrix) * it_light->lightAsEyeMatrix), 1).x;
				});
				auto minmax_y_shadow = std::minmax_element(it->points.begin(), it->points.end(), [&](const Vector3D& a, const Vector3D& b) {
					return lib3d::projectPoint(((a * eyeMatrix) * it_light->lightAsEyeMatrix), 1).y < lib3d::projectPoint(((b * eyeMatrix) * it_light->lightAsEyeMatrix), 1).y;
				});

				min_x_shadow = std::min(min_x_shadow, lib3d::projectPoint(*minmax_x_shadow.first * eyeMatrix * it_light->lightAsEyeMatrix, 1).x);
				max_x_shadow = std::max(max_x_shadow, lib3d::projectPoint(*minmax_x_shadow.second* eyeMatrix * it_light->lightAsEyeMatrix, 1).x);

				min_y_shadow = std::min(min_y_shadow, lib3d::projectPoint(*minmax_y_shadow.first* eyeMatrix * it_light->lightAsEyeMatrix, 1).y);
				max_y_shadow = std::max(max_y_shadow, lib3d::projectPoint(*minmax_y_shadow.second* eyeMatrix * it_light->lightAsEyeMatrix, 1).y);
			}

			double range_x_shadow = max_x_shadow - min_x_shadow;
			double range_y_shadow = max_y_shadow - min_y_shadow;

			double image_x_shadow = shadowSize * range_x_shadow / std::max(range_x_shadow, range_y_shadow);
			double image_y_shadow = shadowSize * range_y_shadow / std::max(range_x_shadow, range_y_shadow);

			double d_shadow = 0.95 * (image_x_shadow / range_x_shadow);

			double offset_x_shadow = (image_x_shadow / 2.0) - (d_shadow * (min_x_shadow + max_x_shadow) / 2.0);
			double offset_y_shadow = (image_y_shadow / 2.0) - (d_shadow * (min_y_shadow + max_y_shadow) / 2.0);

			it_light->shadowmask = lib3d::ZBuffer(roundToInt(image_x_shadow), roundToInt(image_y_shadow));

			it_light->d = d_shadow;
			it_light->dx = offset_x_shadow;
			it_light->dy = offset_y_shadow;

			for (std::vector<lib3d::Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
			{
				for (std::vector<lib3d::Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); ++it_face)
				{
					for (std::vector<unsigned int>::iterator it_face_pt_index = std::next(it_face->point_indexes.begin()); it_face_pt_index != std::prev(it_face->point_indexes.end()); it_face_pt_index++)
					{
						it_light->addShadowTriangle(
							it_figure->points[*it_face->point_indexes.begin()] * eyeMatrix * it_light->lightAsEyeMatrix,
							it_figure->points[*it_face_pt_index] * eyeMatrix * it_light->lightAsEyeMatrix,
							it_figure->points[*std::next(it_face_pt_index)] * eyeMatrix * it_light->lightAsEyeMatrix);
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
					it_figure->points[*it_face->point_indexes.begin()],
					it_figure->points[*it_face_pt_index],
					it_figure->points[*std::next(it_face_pt_index)],
					d,
					offset_x,
					offset_y,
					it_figure->ambientReflection,
					it_figure->diffuseReflection,
					it_figure->specularReflection,
					it_figure->reflectionCoefficient,
					lights,
					shadow,
					eyeMatrix);
			}
		}
	}

	return image;
}