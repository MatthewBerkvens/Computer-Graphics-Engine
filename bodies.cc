//
// Created by matthew.
//

#include "bodies.h"
#include <algorithm>
#include <assert.h>

void bodies::createCube(lib3d::Figure& figure, img::Color& color) //CONFIRMED ANTI-CLOCKWISE
{
	figure.points = {
		Vector3D().point(1, -1, -1),
		Vector3D().point(-1,  1, -1),
		Vector3D().point(1,  1,  1),
		Vector3D().point(-1, -1,  1),
		Vector3D().point(1,  1, -1),
		Vector3D().point(-1, -1, -1),
		Vector3D().point(1, -1,  1),
		Vector3D().point(-1,  1,  1)
	};

	figure.faces = {
		lib3d::Face({ 0, 4, 2, 6 }, color),
		lib3d::Face({ 4, 1, 7, 2 }, color),
		lib3d::Face({ 1, 5, 3, 7 }, color),
		lib3d::Face({ 5, 0, 6, 3 }, color),
		lib3d::Face({ 6, 2, 7, 3 }, color),
		lib3d::Face({ 0, 5, 1, 4 }, color)
	};
}

void bodies::createTetrahedron(lib3d::Figure& figure, img::Color& color) //CONFIRMED ANTI-CLOCKWISE
{
	figure.points = {
		Vector3D().point(1, -1, -1),
		Vector3D().point(-1,  1, -1),
		Vector3D().point(1,  1,  1),
		Vector3D().point(-1, -1,  1)
	};

	figure.faces = {
		lib3d::Face({ 0, 1, 2 }, color),
		lib3d::Face({ 1, 3, 2 }, color),
		lib3d::Face({ 0, 3, 1 }, color),
		lib3d::Face({ 0, 2, 3 }, color)
	};
}

void bodies::createOctahedron(lib3d::Figure& figure, img::Color& color) //CONFIRMED ANTI-CLOCKWISE
{
	figure.points = {
		Vector3D().point(1,  0,  0),
		Vector3D().point(0,  1,  0),
		Vector3D().point(-1,  0,  0),
		Vector3D().point(0, -1,  0),
		Vector3D().point(0,  0, -1),
		Vector3D().point(0,  0,  1),
	};

	figure.faces = {
		lib3d::Face({ 0, 1, 5 }, color),
		lib3d::Face({ 1, 2, 5 }, color),
		lib3d::Face({ 2, 3, 5 }, color),
		lib3d::Face({ 3, 0, 5 }, color),
		lib3d::Face({ 1, 0, 4 }, color),
		lib3d::Face({ 2, 1, 4 }, color),
		lib3d::Face({ 3, 2, 4 }, color),
		lib3d::Face({ 0, 3, 4 }, color)
	};
}

void bodies::createIcosahedron(lib3d::Figure& figure, img::Color& color)
{
	std::vector<Vector3D> points;

	points.push_back(Vector3D().point(0, 0, sqrt(5) / 2));
	for (unsigned int i = 1; i < 6; i++)
	{
		points.push_back(Vector3D().point(
			std::cos((i - 1) * 2 * MY_PI / 5),
			std::sin((i - 1) * 2 * MY_PI / 5),
			0.5));
	}

	for (unsigned int i = 6; i < 11; i++)
	{
		points.push_back(Vector3D().point(
			std::cos((MY_PI / 5) + ((i - 6) * 2 * MY_PI / 5)),
			std::sin((MY_PI / 5) + ((i - 6) * 2 * MY_PI / 5)),
			-0.5));
	}
	points.push_back(Vector3D().point(0, 0, -sqrt(5) / 2));

	figure.points = points;

	figure.faces = {
		lib3d::Face({ 0, 1, 2 }, color),
		lib3d::Face({ 0, 2, 3 }, color),
		lib3d::Face({ 0, 3, 4 }, color),
		lib3d::Face({ 0, 4, 5 }, color),
		lib3d::Face({ 0, 5, 1 }, color),
		lib3d::Face({ 1, 6, 2 }, color),
		lib3d::Face({ 2, 6, 7 }, color),
		lib3d::Face({ 2, 7, 3 }, color),
		lib3d::Face({ 3, 7, 8 }, color),
		lib3d::Face({ 3, 8, 4 }, color),
		lib3d::Face({ 4, 8, 9 }, color),
		lib3d::Face({ 4, 9, 5 }, color),
		lib3d::Face({ 5, 9, 10 }, color),
		lib3d::Face({ 5, 10, 1 }, color),
		lib3d::Face({ 1, 10, 6 }, color),
		lib3d::Face({ 11, 7, 6 }, color),
		lib3d::Face({ 11, 8, 7 }, color),
		lib3d::Face({ 11, 9, 8 }, color),
		lib3d::Face({ 11, 10, 9 }, color),
		lib3d::Face({ 11, 6, 10 }, color)
	};
}

void bodies::createDodecahedron(lib3d::Figure& figure, img::Color& color)
{
	createIcosahedron(figure, color);

	std::vector<Vector3D> newPoints;

	for (std::vector<lib3d::Face>::iterator it_face = figure.faces.begin(); it_face != figure.faces.end(); ++it_face)
	{
		Vector3D pt1 = figure.points[it_face->point_indexes[0]];
		Vector3D pt2 = figure.points[it_face->point_indexes[1]];
		Vector3D pt3 = figure.points[it_face->point_indexes[2]];

		newPoints.push_back(Vector3D().point(
			(pt1.x + pt2.x + pt3.x) / 3,
			(pt1.y + pt2.y + pt3.y) / 3,
			(pt1.z + pt2.z + pt3.z) / 3
		));
	}

	figure.points = newPoints;

	figure.faces = {
		lib3d::Face({ 0, 1, 2, 3, 4 }, color),
		lib3d::Face({ 0, 5, 6, 7, 1 }, color),
		lib3d::Face({ 1, 7, 8, 9, 2 }, color),
		lib3d::Face({ 2, 9, 10, 11, 3 }, color),
		lib3d::Face({ 3, 11, 12, 13, 4 }, color),
		lib3d::Face({ 4, 13, 14, 5, 0 }, color),
		lib3d::Face({ 19, 18, 17, 16, 15 }, color),
		lib3d::Face({ 19, 14, 13, 12, 18 }, color),
		lib3d::Face({ 18, 12, 11, 10, 17 }, color),
		lib3d::Face({ 17, 10, 9, 8, 16 }, color),
		lib3d::Face({ 16, 8, 7, 6, 15 }, color),
		lib3d::Face({ 15, 6, 5, 14, 19 }, color),
	};
}

void bodies::createCone(lib3d::Figure& figure, const unsigned int n, const double h, img::Color& color)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;
	lib3d::Face groundFace(color);

	points.push_back(Vector3D().point(0, 0, h));

	for (unsigned int i = 0; i <= n; i++)
	{
		points.push_back(Vector3D().point(
			std::cos(i * 2 * MY_PI / n),
			std::sin(i * 2 * MY_PI / n),
			0
		));
	}

	for (unsigned i = 1; i < n; i++)
	{
		faces.push_back(lib3d::Face({ (i + 1) % (n + 1), i, 0 }, color));
	}

	for (unsigned int i = n; i > 0; i--)
	{
		groundFace.point_indexes.push_back(i);
	}

	faces.push_back(groundFace);

	figure.faces = faces;
	figure.points = points;
}

void bodies::createCylinder(lib3d::Figure& figure, const unsigned int n, const double h, img::Color& color)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;

	lib3d::Face groundFace(color);
	lib3d::Face ceilingFace(color);

	for (unsigned int i = 0; i < n; i++)
	{
		points.push_back(Vector3D().point(
			std::cos(i * 2 * MY_PI / n),
			std::sin(i * 2 * MY_PI / n),
			0
		));
	}

	for (unsigned int i = 0; i < n; i++)
	{
		points.push_back(Vector3D().point(
			std::cos((i % n) * 2 * MY_PI / n),
			std::sin((i % n) * 2 * MY_PI / n),
			h
		));
	}

	for (unsigned int i = n; i > 0; i--)
	{
		if (i == n)
		{
			faces.push_back(lib3d::Face({ n, 0, n - 1, (2 * n) - 1 }, color));
		}
		else
		{
			faces.push_back(lib3d::Face({ n + i, i, i - 1, n + i - 1 }, color));
		}

		groundFace.point_indexes.push_back(i - 1);
		ceilingFace.point_indexes.push_back(n + i - 1);
	}

	faces.push_back(groundFace);
	faces.push_back(ceilingFace);

	figure.faces = faces;
	figure.points = points;
}

void bodies::createSphere(lib3d::Figure& figure, const unsigned int n, img::Color& color)
{
	createIcosahedron(figure, color);

	for (unsigned int i = 0; i < n; i++)
	{
		std::vector<Vector3D> newPoints;
		std::vector<lib3d::Face> newFaces;

		for (std::vector<lib3d::Face>::iterator it_face = figure.faces.begin(); it_face != figure.faces.end(); it_face++)
		{
			Vector3D A = figure.points[it_face->point_indexes[0]];

			Vector3D B = figure.points[it_face->point_indexes[1]];

			Vector3D C = figure.points[it_face->point_indexes[2]];

			Vector3D D = (A + B) / 2;

			Vector3D E = (A + C) / 2;

			Vector3D F = (C + B) / 2;

			unsigned int size = newPoints.size();
			newPoints.push_back(A); //size + 0
			newPoints.push_back(B); //size + 1
			newPoints.push_back(C); //size + 2

			newPoints.push_back(D); //size + 3
			newPoints.push_back(E); //size + 4
			newPoints.push_back(F); //size + 5


			newFaces.push_back(lib3d::Face({ size + 0, size + 4, size + 3 }, color)); //A - E - D

			newFaces.push_back(lib3d::Face({ size + 1, size + 3, size + 5 }, color)); //B - D - F

			newFaces.push_back(lib3d::Face({ size + 2, size + 5, size + 4 }, color)); //C - F - E

			newFaces.push_back(lib3d::Face({ size + 3, size + 4, size + 5 }, color)); //D - E - F
		}

		figure.points = newPoints;
		figure.faces = newFaces;
	}

	for (std::vector<Vector3D>::iterator it_point = figure.points.begin(); it_point != figure.points.end(); it_point++)
	{
		it_point->normalise();
	}
}

void bodies::createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m, img::Color& color)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;

	double v = 0;
	double u = 0;

	for (unsigned int i = 0; i < n; i++)
	{
		u = 2.0 * MY_PI * (double)i / (double)n;
		for (unsigned int j = 0; j < m; j++)
		{
			v = 2.0 * MY_PI * (double)j / (double)m;
			points.push_back(Vector3D().point(
				(R + (r * std::cos(v))) * std::cos(u),
				(R + (r * std::cos(v))) * std::sin(u),
				r * std::sin(v)
			));

			faces.push_back(lib3d::Face({
				i*m + j,
				i*m + ((j + 1) % m),
				((i + 1) % n)*m + ((j + 1) % m),
				((i + 1) % n)*m + j
				}, color));
		}
	}

	figure.points = points;
	figure.faces = faces;
}

void bodies::createBuckyBall(lib3d::Figure& figure, img::Color& color)
{
	createIcosahedron(figure, color);

	lib3d::Figure hexagons;
	lib3d::Figure triangles;
	lib3d::Figure pentagons;

	for (std::vector<lib3d::Face>::iterator it = figure.faces.begin(); it != figure.faces.end(); it++)
	{
		assert(it->point_indexes.size() == 3);

		lib3d::Face newHexagon(color);

		for (size_t i = 0; i < 3; i++)
		{
			lib3d::Face newTriangle(color);
			newTriangle.point_indexes.push_back(triangles.points.size());
			newTriangle.point_indexes.push_back(triangles.points.size() + 1);
			newTriangle.point_indexes.push_back(triangles.points.size() + 2);

			triangles.points.push_back(figure.points[it->point_indexes[i]]);
			triangles.points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 1) % 3]] - figure.points[it->point_indexes[i]]) / 3));
			triangles.points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 2) % 3]] - figure.points[it->point_indexes[i]]) / 3));

			triangles.faces.push_back(newTriangle);

			newHexagon.point_indexes.push_back(hexagons.points.size() + 1);
			newHexagon.point_indexes.push_back(hexagons.points.size());

			hexagons.points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 1) % 3]] - figure.points[it->point_indexes[i]]) / 3));
			hexagons.points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 2) % 3]] - figure.points[it->point_indexes[i]]) / 3));
		}

		hexagons.faces.push_back(newHexagon);
	}

	for (std::vector<lib3d::Face>::iterator it_triangle_face = triangles.faces.begin(); it_triangle_face != triangles.faces.end(); it_triangle_face++)
	{
		assert(it_triangle_face->point_indexes.size() == 3);

		auto pentagon_face = std::find_if(pentagons.faces.begin(), pentagons.faces.end(), [&](const lib3d::Face face)
		{
			return pentagons.points[face.point_indexes[0]] == triangles.points[it_triangle_face->point_indexes[0]];
		}); //try to find a pentagon face where triangle_point[current_triangle_face_point_index[0]] equals to pentagons_points[the_pentagon_face_we_seek_point_index[0]] 

		if (pentagon_face == pentagons.faces.end()) //if face was not found we need to make a new face with the current points, including point 0 as reference
		{
			pentagons.faces.push_back(lib3d::Face({ (unsigned int)pentagons.points.size(), (unsigned int)pentagons.points.size() + (unsigned int)1, (unsigned int)pentagons.points.size() + (unsigned int)2 }, color));

			pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[0]]);
			pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[1]]);
			pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[2]]);
		}
		else
		{
			auto pt1 = std::find_if(pentagon_face->point_indexes.begin(), pentagon_face->point_indexes.end(), [&](const unsigned int pt)
			{
				return pentagons.points[pt] == triangles.points[it_triangle_face->point_indexes[1]];
			}); //try to find pt 1 of current triangle face

			auto pt2 = std::find_if(pentagon_face->point_indexes.begin(), pentagon_face->point_indexes.end(), [&](const unsigned int pt)
			{
				return pentagons.points[pt] == triangles.points[it_triangle_face->point_indexes[2]];
			}); //try to find pt 2 of current triangle face



			if (pt1 == pentagon_face->point_indexes.end() && pt2 != pentagon_face->point_indexes.end()) //pt1 not found in the found pentagon face, so only push triangle_pt2
			{
				pentagon_face->point_indexes.insert(pt2, pentagons.points.size()); //insert triangle_pt1 before pt2
				pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[1]]);
			}
			else if (pt1 != pentagon_face->point_indexes.end() && pt2 == pentagon_face->point_indexes.end()) //pt2 not found in the found pentagon face, so only push triangle_pt1
			{
				pentagon_face->point_indexes.insert(std::next(pt1), pentagons.points.size()); //insert triangle_pt2 after pt1
				pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[2]]);
			}
			else if (pt1 == pentagon_face->point_indexes.end() && pt2 == pentagon_face->point_indexes.end()) //both not found in the found pentagon face, push both.
			{
				pentagon_face->point_indexes.push_back(pentagons.points.size());
				pentagon_face->point_indexes.push_back(pentagons.points.size() + 1);
				pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[1]]);
				pentagons.points.push_back(triangles.points[it_triangle_face->point_indexes[2]]);
			}

			assert(pentagon_face->point_indexes.size() <= 6);
		}
	}

	std::vector<Vector3D> newPoints; //remove useless point
	for (std::vector<Vector3D>::iterator it_pentagon_pt = pentagons.points.begin(); it_pentagon_pt != pentagons.points.end(); it_pentagon_pt++)
	{
		if (std::find_if(pentagons.faces.begin(), pentagons.faces.end(), [&](const lib3d::Face face) {
			return (std::find_if(std::next(face.point_indexes.begin()), face.point_indexes.end(), [&](const unsigned int pt_index) {
				return pentagons.points[pt_index] == *it_pentagon_pt;
			}) != face.point_indexes.end());
		}) != pentagons.faces.end())
			newPoints.push_back(*it_pentagon_pt);
	}

	std::vector<lib3d::Face> newFaces; //remove point 0 from the faces we just generated
	for (std::vector<lib3d::Face>::iterator it_pentagon_face = pentagons.faces.begin(); it_pentagon_face != pentagons.faces.end(); it_pentagon_face++)
	{
		lib3d::Face newFace(color);
		for (std::vector<unsigned int>::iterator it_pt_index = std::next(it_pentagon_face->point_indexes.begin()); it_pt_index != it_pentagon_face->point_indexes.end(); it_pt_index++)
		{
			for (size_t i = 0; i < newPoints.size(); i++)
			{
				if (newPoints[i] == pentagons.points[*it_pt_index])
					newFace.point_indexes.push_back(i);
			}
		}
		assert(newFace.point_indexes.size() == 5);
		newFaces.push_back(newFace);
	}

	pentagons.points = newPoints;
	pentagons.faces = newFaces;

	std::vector<lib3d::Figure> combined;
	combined.push_back(pentagons);
	combined.push_back(hexagons);
	figure.points.clear();
	figure.faces.clear();
	lib3d::combineFigures(figure, combined);
}