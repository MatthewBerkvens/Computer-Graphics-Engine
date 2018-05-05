//
// Created by matthew
//

#ifndef ENGINE_LIB3D_H
#define ENGINE_LIB3D_H
#include "vector3d.h"
#include "mylibrary.h"
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

		Face(std::vector<unsigned int> _point_indexes) : point_indexes(_point_indexes) {}

		Face() : point_indexes({}) {}
	};

	class Figure
	{
	public:
		std::vector<Vector3D> points;
		std::vector<Face> faces;

		std::vector<double> ambientReflection;
		std::vector<double> diffuseReflection;
		std::vector<double> specularReflection;
		double reflectionCoefficient;

		Figure(std::vector<double>& _ambientReflection, std::vector<double>& _diffuseReflection, std::vector<double>& _specularReflection, double _reflectionCoefficient) :
			ambientReflection(_ambientReflection), diffuseReflection(_diffuseReflection), specularReflection(_specularReflection), reflectionCoefficient(_reflectionCoefficient) 
		{
			assert(_ambientReflection.size() == 3);
			assert(_diffuseReflection.size() == 3);
			assert(_specularReflection.size() == 3);
		}

		void triangulateFigure();
	};

	class Light
	{
	public:
		std::vector<double> ambientLight;
		std::vector<double> diffuseLight;
		std::vector<double> specularLight;

		bool specialLight;
		bool infinity;

		Vector3D ldVector;
		Vector3D location;

		Light(std::vector<double>& _ambientLight, std::vector<double>& _diffuseLight, std::vector<double>& _specularLight, bool _specialLight, bool _infinity, Vector3D& _ldVector, Vector3D& _location) :
			ambientLight(_ambientLight), diffuseLight(_diffuseLight), specularLight(_specularLight), specialLight(_specialLight), infinity(_infinity), ldVector(_ldVector), location(_location)
		{
			assert(_ambientLight.size() == 3);
			assert(_diffuseLight.size() == 3);
			assert(_specularLight.size() == 3);
		}
	};


	Matrix scaleMatrix(const double scale);

	Matrix rotateXMatrix(const double angle);

	Matrix rotateYMatrix(const double angle);

	Matrix rotateZMatrix(const double angle);

	Matrix translateMatrix(const Vector3D& vector);

	Matrix transformEyePointMatrix(const Vector3D& eyepoint);

	std::tuple<double, double, double> toPolar(const Vector3D& point);

	Color colorFromNormalizedDoubleTuple(std::vector<double> colorNormalized);



	void transformFigure(Figure& figure, const Matrix& transformMatrix);

	void applyEyeTransform(std::vector<Figure>& figures, const Vector3D& eyepoint);

	std::pair<std::vector<Point2D>, std::vector<Line2D>> projectFigures(std::vector<Figure>& figures, const double d);

	Point2D projectPoint(const Vector3D& point, const double d);

	void combineFigures(Figure& out, std::vector<Figure>& figures);
}

#endif //ENGINE_LIB3D_H
