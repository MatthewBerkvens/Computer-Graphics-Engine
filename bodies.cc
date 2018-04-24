//
// Created by matthew.
//

#include "bodies.h"
#include <assert.h>

void bodies::createCube(lib3d::Figure& figure)
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
		lib3d::Face({ 0, 4, 2, 6 }),
		lib3d::Face({ 4, 1, 7, 2 }),
		lib3d::Face({ 1, 5, 3, 7 }),
		lib3d::Face({ 5, 0, 6, 3 }),
		lib3d::Face({ 6, 2, 7, 3 }),
		lib3d::Face({ 0, 5, 1, 4 })
	};
}

void bodies::createTetrahedron(lib3d::Figure& figure)
{
	figure.points = {
		Vector3D().point(1, -1, -1),
		Vector3D().point(-1,  1, -1),
		Vector3D().point(1,  1,  1),
		Vector3D().point(-1, -1,  1)
	};

	figure.faces = {
		lib3d::Face({ 0, 1, 2 }),
		lib3d::Face({ 1, 3, 2 }),
		lib3d::Face({ 0, 3, 1 }),
		lib3d::Face({ 0, 2, 3 })
	};
}

void bodies::createOctahedron(lib3d::Figure& figure)
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
		lib3d::Face({ 0, 1, 5 }),
		lib3d::Face({ 1, 2, 5 }),
		lib3d::Face({ 2, 3, 5 }),
		lib3d::Face({ 3, 0, 5 }),
		lib3d::Face({ 1, 0, 4 }),
		lib3d::Face({ 2, 1, 4 }),
		lib3d::Face({ 3, 2, 4 }),
		lib3d::Face({ 0, 3, 4 })
	};
}

void bodies::createIcosahedron(lib3d::Figure& figure)
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
		lib3d::Face({ 0, 1, 2 }),
		lib3d::Face({ 0, 2, 3 }),
		lib3d::Face({ 0, 3, 4 }),
		lib3d::Face({ 0, 4, 5 }),
		lib3d::Face({ 0, 5, 1 }),
		lib3d::Face({ 1, 6, 2 }),
		lib3d::Face({ 2, 6, 7 }),
		lib3d::Face({ 2, 7, 3 }),
		lib3d::Face({ 3, 7, 8 }),
		lib3d::Face({ 3, 8, 4 }),
		lib3d::Face({ 4, 8, 9 }),
		lib3d::Face({ 4, 9, 5 }),
		lib3d::Face({ 5, 9, 10 }),
		lib3d::Face({ 5, 10, 1 }),
		lib3d::Face({ 1, 10, 6 }),
		lib3d::Face({ 11, 7, 6 }),
		lib3d::Face({ 11, 8, 7 }),
		lib3d::Face({ 11, 9, 8 }),
		lib3d::Face({ 11, 10, 9 }),
		lib3d::Face({ 11, 6, 10 })
	};
}

void bodies::createDodecahedron(lib3d::Figure& figure)
{
	createIcosahedron(figure);

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
		lib3d::Face({ 0, 1, 2, 3, 4 }),
		lib3d::Face({ 0, 5, 6, 7, 1 }),
		lib3d::Face({ 1, 7, 8, 9, 2 }),
		lib3d::Face({ 2, 9, 10, 11, 3 }),
		lib3d::Face({ 3, 11, 12, 13, 4 }),
		lib3d::Face({ 4, 13, 14, 5, 0 }),
		lib3d::Face({ 19, 18, 17, 16, 15 }),
		lib3d::Face({ 19, 14, 13, 12, 18 }),
		lib3d::Face({ 18, 12, 11, 10, 17 }),
		lib3d::Face({ 17, 10, 9, 8, 16 }),
		lib3d::Face({ 16, 8, 7, 6, 15 }),
		lib3d::Face({ 15, 6, 5, 14, 19 }),
	};
}

void bodies::createCone(lib3d::Figure& figure, const unsigned int n, const double h)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;
	lib3d::Face groundFace;

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
		faces.push_back(lib3d::Face({ (i + 1) % (n + 1), i, 0 }));
	}

	for (unsigned int i = n; i > 0; i--)
	{
		groundFace.point_indexes.push_back(i);
	}

	faces.push_back(groundFace);

	figure.faces = faces;
	figure.points = points;
}

void bodies::createCylinder(lib3d::Figure& figure, const unsigned int n, const double h)
{
	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;

	lib3d::Face groundFace;
	lib3d::Face ceilingFace;

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
			faces.push_back(lib3d::Face({ n, 0, n - 1, (2 * n) - 1 }));
		}
		else
		{
			faces.push_back(lib3d::Face({ n + i, i, i - 1, n + i - 1 }));
		}

		groundFace.point_indexes.push_back(i - 1);
		ceilingFace.point_indexes.push_back(n + i - 1);
	}

	faces.push_back(groundFace);
	faces.push_back(ceilingFace);

	figure.faces = faces;
	figure.points = points;
}

void bodies::createSphere(lib3d::Figure& figure, const unsigned int n)
{
	createIcosahedron(figure);

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


			newFaces.push_back(lib3d::Face({ size + 0, size + 4, size + 3 })); //A - E - D

			newFaces.push_back(lib3d::Face({ size + 1, size + 3, size + 5 })); //B - D - F

			newFaces.push_back(lib3d::Face({ size + 2, size + 5, size + 4 })); //C - F - E

			newFaces.push_back(lib3d::Face({ size + 3, size + 4, size + 5 })); //D - E - F
		}

		figure.points = newPoints;
		figure.faces = newFaces;
	}

	for (std::vector<Vector3D>::iterator it_point = figure.points.begin(); it_point != figure.points.end(); it_point++)
	{
		it_point->normalise();
	}
}

void bodies::createTorus(lib3d::Figure& figure, const double r, const double R, const unsigned int n, const unsigned int m)
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
				}));
		}
	}

	figure.points = points;
	figure.faces = faces;
}

void bodies::createBuckyBall(lib3d::Figure& figure)
{
	createIcosahedron(figure);

	std::vector<Vector3D> points;
	std::vector<lib3d::Face> faces;

	for (std::vector<lib3d::Face>::iterator it = figure.faces.begin(); it != figure.faces.end(); it++)
	{
		assert(it->point_indexes.size() == 3);

		lib3d::Face six;

		for (size_t i = 0; i < 3; i++)
		{
			six.point_indexes.push_back(points.size() + 1);
			six.point_indexes.push_back(points.size());

			points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 1) % 3]] - figure.points[it->point_indexes[i]]) / 3));
			points.push_back(figure.points[it->point_indexes[i]] + ((figure.points[it->point_indexes[(i + 2) % 3]] - figure.points[it->point_indexes[i]]) / 3));
		}

		faces.push_back(six);
	}

	figure.points = points;
	figure.faces = faces;
}