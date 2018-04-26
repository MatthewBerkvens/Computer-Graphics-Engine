//
// Created by matthew on 06.03.18.
//

#include "lib3d.h"
#include "vector3d.h"
#include "mylibrary.h"
#include <assert.h>
#include <vector>

namespace lib3d
{
	Matrix scaleMatrix(const double scale)
	{
		Matrix matrix;
		matrix(1, 1) = scale;
		matrix(2, 2) = scale;
		matrix(3, 3) = scale;
		return matrix;
	}

	Matrix rotateXMatrix(const double angle)
	{
		Matrix matrix;
		matrix(2, 2) = std::cos(angle);
		matrix(2, 3) = std::sin(angle);
		matrix(3, 2) = -std::sin(angle);
		matrix(3, 3) = std::cos(angle);
		return matrix;
	}

	Matrix rotateYMatrix(const double angle)
	{
		Matrix matrix;
		matrix(1, 1) = std::cos(angle);
		matrix(1, 3) = -std::sin(angle);
		matrix(3, 1) = std::sin(angle);
		matrix(3, 3) = std::cos(angle);
		return matrix;
	}

	Matrix rotateZMatrix(const double angle)
	{
		Matrix matrix;
		matrix(1, 1) = std::cos(angle);
		matrix(1, 2) = std::sin(angle);
		matrix(2, 1) = -std::sin(angle);
		matrix(2, 2) = std::cos(angle);
		return matrix;
	}

	Matrix translateMatrix(const Vector3D& vector)
	{
		assert(vector.is_vector());
		Matrix matrix;
		matrix(4, 1) = vector.x;
		matrix(4, 2) = vector.y;
		matrix(4, 3) = vector.z;
		return matrix;
	}

	Matrix transformEyePointMatrix(const Vector3D& eyepoint)
	{
		Matrix matrix;
		double r = eyepoint.length();
		double theta = std::atan2(eyepoint.y, eyepoint.x);
		double phi = std::acos(eyepoint.z / r);

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


	Face::Face(std::vector<unsigned int> point_indexes)
	{
		this->point_indexes = point_indexes;
	}

	Face::Face() {};

	std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d)
	{
		std::vector<Line2D> lines;
		std::vector<Point2D> points;

		for (std::vector<Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); ++it_figure)
		{
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

					Point2D a_pt = projectPoint(a_vec, d);
					Point2D b_pt = projectPoint(b_vec, d);

					points.push_back(a_pt);
					points.push_back(b_pt);

					lines.push_back(Line2D(a_pt, a_vec.z, b_pt, b_vec.z, it_figure->color));
				}
			}
		}

		return std::pair<std::vector<Point2D>, std::vector<Line2D>>(points, lines);
	}


	void transformFigure(Figure& figure, const Matrix& transformMatrix)
	{
		for (std::vector<Vector3D>::iterator it = figure.points.begin(); it != figure.points.end(); ++it)
		{
			(*it) *= transformMatrix;
		}
	}

	void applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint)
	{
		Matrix eyePointMatrix = transformEyePointMatrix(eyepoint);

		for (std::vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it)
		{
			transformFigure(*it, eyePointMatrix);
		}
	}

	Point2D projectPoint(const Vector3D& point, const double d)
	{
		return Point2D(d*(point.x / -point.z), d*(point.y / -point.z));
	}

	Figure combineFigures(std::vector<lib3d::Figure>& figures)
	{
		assert(figures.size() > 0);
		Figure newFigure;
		newFigure.color = figures[0].color;

		for (std::vector<Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); it_figure++)
		{
			unsigned int offset = newFigure.points.size();

			for (std::vector<Face>::iterator it_face = it_figure->faces.begin(); it_face != it_figure->faces.end(); it_face++)
			{
				Face newFace;
				for (std::vector<unsigned int>::iterator it_face_index = it_face->point_indexes.begin(); it_face_index != it_face->point_indexes.end(); it_face_index++)
				{
					newFace.point_indexes.push_back(*it_face_index + offset);
				}
				newFigure.faces.push_back(newFace);
			}

			newFigure.points.insert(newFigure.points.end(), it_figure->points.begin(), it_figure->points.end());
		}

		return newFigure;
	}
}