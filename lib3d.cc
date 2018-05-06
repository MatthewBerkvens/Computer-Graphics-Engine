//
// Created by matthew on 06.03.18.
//

#include "lib3d.h"


lib3d::Color::Color() : blue(0), green(0), red(0)
{
}
lib3d::Color::Color(uint8_t r, uint8_t g, uint8_t b) : blue(b), green(g), red(r)
{
}
lib3d::Color::~Color()
{
}

Matrix lib3d::scaleMatrix(const double scale)
{
	Matrix matrix;
	matrix(1, 1) = scale;
	matrix(2, 2) = scale;
	matrix(3, 3) = scale;
	return matrix;
}

Matrix lib3d::rotateXMatrix(const double angle)
{
	Matrix matrix;
	matrix(2, 2) = std::cos(angle);
	matrix(2, 3) = std::sin(angle);
	matrix(3, 2) = -std::sin(angle);
	matrix(3, 3) = std::cos(angle);
	return matrix;
}

Matrix lib3d::rotateYMatrix(const double angle)
{
	Matrix matrix;
	matrix(1, 1) = std::cos(angle);
	matrix(1, 3) = -std::sin(angle);
	matrix(3, 1) = std::sin(angle);
	matrix(3, 3) = std::cos(angle);
	return matrix;
}

Matrix lib3d::rotateZMatrix(const double angle)
{
	Matrix matrix;
	matrix(1, 1) = std::cos(angle);
	matrix(1, 2) = std::sin(angle);
	matrix(2, 1) = -std::sin(angle);
	matrix(2, 2) = std::cos(angle);
	return matrix;
}

Matrix lib3d::translateMatrix(const Vector3D& vector)
{
	assert(vector.is_vector());
	Matrix matrix;
	matrix(4, 1) = vector.x;
	matrix(4, 2) = vector.y;
	matrix(4, 3) = vector.z;
	return matrix;
}

Matrix lib3d::transformEyePointMatrix(const Vector3D& eyepoint)
{
	Matrix matrix;

	std::tuple<double, double, double> polar = lib3d::toPolar(eyepoint);

	double r = std::get<0>(polar);
	double theta = std::get<1>(polar);
	double phi = std::get<2>(polar);

	matrix(1, 1) = -std::sin(theta);
	matrix(1, 2) = -std::cos(theta) * std::cos(phi);
	matrix(1, 3) = std::cos(theta) * std::sin(phi);

	matrix(2, 1) = std::cos(theta);
	matrix(2, 2) = -std::sin(theta) * std::cos(phi);
	matrix(2, 3) = std::sin(theta) * std::sin(phi);

	matrix(3, 2) = std::sin(phi);
	matrix(3, 3) = std::cos(phi);

	matrix(4, 3) = -r;

	return matrix;
}

std::tuple<double, double, double> lib3d::toPolar(const Vector3D& point)
{
	double r = point.length();
	double theta = std::atan2(point.y, point.x);
	double phi = std::acos(point.z / r);
	return std::tuple<double, double, double>(r, theta, phi);
}

lib3d::Color lib3d::colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized)
{
	if (colorNormalized.size() != 3)
		throw std::invalid_argument("expected size 3");
	return lib3d::Color((uint8_t)roundToInt(colorNormalized[0] * 255), (uint8_t)roundToInt(colorNormalized[1] * 255), (uint8_t)roundToInt(colorNormalized[2] * 255));
}



std::pair<std::vector<lib3d::Point2D>, std::vector<lib3d::Line2D>> lib3d::projectFigures(std::vector<Figure>& figures, const double d)
{
	std::vector<lib3d::Line2D> lines;
	std::vector<lib3d::Point2D> points;

	for (std::vector<Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); ++it_figure)
	{
		lib3d::Color color = colorFromNormalizedDoubleTuple(it_figure->ambientReflection);
		for (std::vector<Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); ++it_face)
		{
			for (std::vector<int>::size_type i = 0; i != it_face->point_indexes.size(); i++)
			{
				Vector3D a_vec;
				Vector3D b_vec;

				if (i == it_face->point_indexes.size() - 1) {
					if (it_face->point_indexes.size() > 2)
					{
						a_vec = it_figure->points[it_face->point_indexes[i]];
						b_vec = it_figure->points[it_face->point_indexes[0]];
						assert(it_face->point_indexes[i] != it_face->point_indexes[0]);
					}
					else continue;
				}
				else
				{
					a_vec = it_figure->points[it_face->point_indexes[i]];
					b_vec = it_figure->points[it_face->point_indexes[i + 1]];
					assert(it_face->point_indexes[i] != it_face->point_indexes[i + 1]);
				}

				lib3d::Point2D a_pt = projectPoint(a_vec, d);
				lib3d::Point2D b_pt = projectPoint(b_vec, d);

				points.push_back(a_pt);
				points.push_back(b_pt);

				lines.push_back(lib3d::Line2D(a_pt, a_vec.z, b_pt, b_vec.z, color));
			}
		}
	}

	return std::pair<std::vector<lib3d::Point2D>, std::vector<lib3d::Line2D>>(points, lines);
}


void lib3d::transformFigure(Figure& figure, const Matrix& transformMatrix)
{
	for (std::vector<Vector3D>::iterator it = figure.points.begin(); it != figure.points.end(); ++it)
	{
		(*it) *= transformMatrix;
	}
}

void lib3d::applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint)
{
	Matrix eyePointMatrix = transformEyePointMatrix(eyepoint);

	for (std::vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it)
	{
		transformFigure(*it, eyePointMatrix);
	}
}

lib3d::Point2D lib3d::projectPoint(const Vector3D& point, const double d)
{
	return lib3d::Point2D(d*(point.x / -point.z), d*(point.y / -point.z));
}

void lib3d::combineFigures(Figure& out, std::vector<Figure>& figures)
{
	for (std::vector<Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
	{
		unsigned int offset = out.points.size();

		for (std::vector<Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); it_face++)
		{
			Face newFace;
			for (std::vector<unsigned int>::iterator it_face_pt_index = it_face->point_indexes.begin(); it_face_pt_index != it_face->point_indexes.end(); it_face_pt_index++)
			{
				newFace.point_indexes.push_back(*it_face_pt_index + offset);
			}
			out.faces.push_back(newFace);
		}

		out.points.insert(out.points.end(), it_figure->points.begin(), it_figure->points.end());
	}
}