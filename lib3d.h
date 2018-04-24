//
// Created by matthew on 06.03.18.
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
		Face(std::vector<unsigned int> point_indexes);
        Face();
    };

    class Figure
    {
    public:
        std::vector<Vector3D> points;
        std::vector<Face> faces;
        img::Color color;
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
}

#endif //ENGINE_LIB3D_H
