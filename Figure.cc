#include "Figure.h"

Figure::Figure(std::vector<double>& _ambientReflection, std::vector<double>& _diffuseReflection, std::vector<double>& _specularReflection, double _reflectionCoefficient)
	: ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient)
{
	assert(_ambientReflection.size() == 3);
	assert(_diffuseReflection.size() == 3);
	assert(_specularReflection.size() == 3);
}

std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d)
{
	std::vector<Line2D> lines;
	std::vector<Point2D> points;

	for (std::vector<Figure>::iterator it_figure = figures.begin(); it_figure != figures.end(); ++it_figure)
	{
		Color color = colorFromNormalizedDoubleTuple(it_figure->ambientReflection);
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

				lines.push_back(Line2D(a_pt, a_vec.z, b_pt, b_vec.z, color));
			}
		}
	}

	return std::pair<std::vector<Point2D>, std::vector<Line2D>>(points, lines);
}

void combineFigures(Figure& out, std::vector<Figure>& figures)
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