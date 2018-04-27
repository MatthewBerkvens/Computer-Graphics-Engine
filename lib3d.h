//
// Created by matthew
//

#ifndef ENGINE_LIB3D_H
#define ENGINE_LIB3D_H
#include "vector3d.h"
#include "easy_image.h"
#include "mylibrary.h"
#include <vector>

namespace lib3d
{
	class Face
	{
	public:
		std::vector<unsigned int> point_indexes;
		img::Color color;
		Face(std::vector<unsigned int> _point_indexes, img::Color _color) : point_indexes(_point_indexes), color(_color) {}
		Face(img::Color _color) : point_indexes({}), color(_color) {}
	};

	class Figure
	{
	public:
		std::vector<Vector3D> points;
		std::vector<Face> faces;
		
		void triangulateFigure();
	};

	Matrix scaleMatrix(const double scale);

	Matrix rotateXMatrix(const double angle);

	Matrix rotateYMatrix(const double angle);

	Matrix rotateZMatrix(const double angle);

	Matrix translateMatrix(const Vector3D& vector);

	Matrix transformEyePointMatrix(const Vector3D& eyepoint);



	void transformFigure(Figure& figure, const Matrix& transformMatrix);

	void applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint);

	std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d);

	Point2D projectPoint(const Vector3D& point, const double d);

	void combineFigures(lib3d::Figure& out, std::vector<lib3d::Figure>& figures);
}

#endif //ENGINE_LIB3D_H
