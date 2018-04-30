//
// Created by matthew
//

#ifndef ENGINE_LIB3D_H
#define ENGINE_LIB3D_H
#include "vector3d.h"
#include <vector>
#include <assert.h>
#include <tuple>
#include <cmath>

namespace lib3d
{
	/**
	* \brief This class represents the color of a pixel in an img::EasyImage object
	*/
	class Color
	{
		//a safety warning to all of you: Do *NOT* rearrange the 'color components' in this class
		//easyimage expects these three fields to be the *first* fields in the class AND expects
		//them to be in *this* order
		//if you alter the arrangement, the generated BMP files will contain garbage
	public:
		/**
		* \brief The intensity of the blue color component
		*/
		uint8_t blue;

		/**
		* \brief The intensity of the green color component
		*/
		uint8_t green;

		/**
		* \brief The intensity of the red color component
		*/
		uint8_t red;

		/**
		* \brief Default Constructor
		*/
		Color();

		/**
		* \brief Constructs a Color with the given intensities
		*
		* \param r	The red color component
		* \param g	The green color component
		* \param b	The blue color component
		*
		*/
		Color(uint8_t r, uint8_t g, uint8_t b);

		/**
		* Destructor
		*/
		~Color();
	};

	Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized);

	class Point2D {
	public:
		double x;
		double y;

		Point2D(double x, double y) : x(x), y(y) {}
	};

	class Line2D {
	public:
		Point2D a;
		double z_a;

		Point2D b;
		double z_b;

		Color color;

		Line2D(Point2D& a, Point2D& b, Color& color) : a(a), b(b), color(color) {}
		Line2D(Point2D& a, double z_a, Point2D& b, double z_b, Color& color) : a(a), z_a(z_a), b(b), z_b(z_b), color(color) {}
	};

	class Face
	{
	public:
		std::vector<unsigned int> point_indexes;
		Color ambientReflection;
		Color diffuseReflection;
		Color specularReflection;
		double reflectionCoefficient;

		Face(const std::vector<unsigned int> _point_indexes, lib3d::Color& _ambientReflection, lib3d::Color& _diffuseReflection, lib3d::Color& _specularReflection, double _reflectionCoefficient) :
			point_indexes(_point_indexes), ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient) {}

		Face(lib3d::Color& _ambientReflection, lib3d::Color& _diffuseReflection, lib3d::Color& _specularReflection, double _reflectionCoefficient) :
			point_indexes({}), ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient) {}
	};

	class Figure
	{
	public:
		std::vector<Vector3D> points;
		std::vector<Face> faces;

		void triangulateFigure();
	};

	class Light
	{
	public:
		std::tuple<double, double, double> ambientLight;
		std::tuple<double, double, double> diffuseLight;
		std::tuple<double, double, double> specularLight;

		bool specialLight;
		bool infinity;

		Vector3D ldVector;
		Vector3D location;

		Light(std::tuple<double, double, double>& _ambientLight, std::tuple<double, double, double>& _diffuseLight, std::tuple<double, double, double>& _specularLight, bool _specialLight, bool _infinity, Vector3D& _ldVector, Vector3D& _location) :
			ambientLight(_ambientLight), diffuseLight(_diffuseLight), specularLight(_specularLight), specialLight(_specialLight), infinity(_infinity), ldVector(_ldVector), location(_location) {}
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
