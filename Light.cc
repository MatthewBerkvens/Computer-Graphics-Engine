#include "Light.h"

Light::Light(std::vector<double>& _ambientLight, std::vector<double>& _diffuseLight, std::vector<double>& _specularLight, bool _specialLight, bool _infinity, Vector3D& _ldVector, Vector3D& _location, Matrix _lightAsEyeMatrix)
	: ambientLight(_ambientLight), diffuseLight(_diffuseLight), specularLight(_specularLight), specialLight(_specialLight), infinity(_infinity), ldVector(_ldVector), location(_location), lightAsEyeMatrix(_lightAsEyeMatrix), shadowmask(0, 0)
{
	assert(_ambientLight.size() == 3);
	assert(_diffuseLight.size() == 3);
	assert(_specularLight.size() == 3);
}

void Light::addShadowTriangle(const Vector3D& A, const Vector3D& B, const Vector3D& C)
{
	Point2D projected_A(((d * A.x) / -A.z) + dx, ((d * A.y) / -A.z) + dy);
	Point2D projected_B(((d * B.x) / -B.z) + dx, ((d * B.y) / -B.z) + dy);
	Point2D projected_C(((d * C.x) / -C.z) + dx, ((d * C.y) / -C.z) + dy);

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
			const Point2D& P = i < 2 ? (i < 1 ? projected_A : projected_B) : projected_C;
			const Point2D& Q = (i + 1) % 3 < 2 ? ((i + 1) % 3 < 1 ? projected_A : projected_B) : projected_C;

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

			if (z_inv < shadowmask[x_cur][y_cur]) shadowmask[x_cur][y_cur] = z_inv;
		}
	}
}

bool Light::isInSight(Vector3D& realWorldPoint)
{
	Vector3D pointFromLightAsEye = realWorldPoint * lightAsEyeMatrix;
	Point2D projected = Point2D(((d * pointFromLightAsEye.x) / -pointFromLightAsEye.z) + dx, ((d * pointFromLightAsEye.y) / -pointFromLightAsEye.z) + dy);

	double a_x = projected.x - std::floor(projected.x);
	double a_y = projected.y - std::floor(projected.y);

	double z_a = shadowmask[(int)std::floor(projected.x)][(int)std::floor(projected.y)];
	double z_b = shadowmask[(int)std::ceil(projected.x)][(int)std::floor(projected.y)];

	double z_c = shadowmask[(int)std::floor(projected.x)][(int)std::ceil(projected.y)];
	double z_d = shadowmask[(int)std::ceil(projected.x)][(int)std::ceil(projected.y)];


	double z_e = ((1.0 - a_x) / z_a) + (a_x / z_b);
	double z_f = ((1.0 - a_x) / z_c) + (a_x / z_d);

	double z_projected = ((1.0 - a_y) / z_e) + (a_y / z_f);

	return std::abs(z_projected - (1 / pointFromLightAsEye.z)) < 1E-5;
}