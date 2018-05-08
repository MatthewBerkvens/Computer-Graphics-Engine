//
// Created by matthew on 06.03.18.
//

#include "lib3d.h"


lib3d::ZBuffer::ZBuffer(const unsigned int width, const unsigned int height) {
	double posInf = std::numeric_limits<double>::infinity();

	for (unsigned int x = 0; x < width; x++)
	{
		this->push_back(std::vector<double>());
		for (unsigned int y = 0; y < height; y++)
		{
			(*this)[x].push_back(posInf);
		}
	}
}

lib3d::Color::Color() : blue(0), green(0), red(0) {}
lib3d::Color::Color(uint8_t r, uint8_t g, uint8_t b) : blue(b), green(g), red(r) {}
lib3d::Color::~Color() {}


void lib3d::Light::addShadowTriangle(const Vector3D& A, const Vector3D& B, const Vector3D& C)
{
	lib3d::Point2D projected_A = lib3d::Point2D(((d * A.x) / -A.z) + dx, ((d * A.y) / -A.z) + dy);
	lib3d::Point2D projected_B = lib3d::Point2D(((d * B.x) / -B.z) + dx, ((d * B.y) / -B.z) + dy);
	lib3d::Point2D projected_C = lib3d::Point2D(((d * C.x) / -C.z) + dx, ((d * C.y) / -C.z) + dy);

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

	double dzdx = w1 / (-d * k);
	double dzdy = w2 / (-d * k);

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

			if (z_inv < shadowmask[x_cur][y_cur])
			{
				shadowmask[x_cur][y_cur] = z_inv;
			}
		}
	}
}

bool lib3d::Light::isInSight(Vector3D& realWorldPoint)
{
	Vector3D pointFromLightAsEye = realWorldPoint * lightAsEyeMatrix;
	Point2D projected = Point2D(((d * pointFromLightAsEye.x) / -pointFromLightAsEye.z) + dx, ((d * pointFromLightAsEye.y) / -pointFromLightAsEye.z) + dy);
	double inf = std::numeric_limits<double>::infinity();
	double inv = shadowmask[roundToInt(projected.x)][roundToInt(projected.y)];
	return std::abs(inv - (1 / pointFromLightAsEye.z)) < 1E-10;
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