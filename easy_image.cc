/*
 * easy_image.cc
 * Copyright (C) 2011  Daniel van den Akker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "easy_image.h"

#define le32toh(x) (x)

namespace img
{
	//structs borrowed from wikipedia's article on the BMP file format
	struct bmpfile_magic
	{
		uint8_t magic[2];
	};

	struct bmpfile_header
	{
		uint32_t file_size;
		uint16_t reserved_1;
		uint16_t reserved_2;
		uint32_t bmp_offset;
	};
	struct bmp_header
	{
		uint32_t header_size;
		int32_t width;
		int32_t height;
		uint16_t nplanes;
		uint16_t bits_per_pixel;
		uint32_t compress_type;
		uint32_t pixel_size;
		int32_t hres;
		int32_t vres;
		uint32_t ncolors;
		uint32_t nimpcolors;
	};
	//copy-pasted from lparser.cc to allow these classes to be used independently from each other
	class enable_exceptions
	{
	private:
		std::ios& ios;
		std::ios::iostate state;
	public:
		enable_exceptions(std::ios& an_ios, std::ios::iostate exceptions) :
			ios(an_ios)
		{
			state = ios.exceptions();
			ios.exceptions(exceptions);
		}
		~enable_exceptions()
		{
			ios.exceptions(state);
		}
	};
	//helper function to convert a number (char, int, ...) to little endian
	//regardless of the endiannes of the system
	//more efficient machine-dependent functions exist, but this one is more portable
	template<typename T> T to_little_endian(T value)
	{
		//yes, unions must be used with caution, but this is a case in which a union is needed
		union
		{
			T t;
			uint8_t bytes[sizeof(T)];
		} temp_storage;

		for (uint8_t i = 0; i < sizeof(T); i++)
		{
			temp_storage.bytes[i] = value & 0xFF;
			value >>= 8;
		}
		return temp_storage.t;
	}

	template<typename T> T from_little_endian(T value)
	{
		//yes, unions must be used with caution, but this is a case in which a union is needed
		union
		{
			T t;
			uint8_t bytes[sizeof(T)];
		} temp_storage;
		temp_storage.t = value;
		T retVal = 0;

		for (uint8_t i = 0; i < sizeof(T); i++)
		{
			retVal = (retVal << 8) | temp_storage.bytes[sizeof(T) - i - 1];
		}
		return retVal;
	}

}

img::UnsupportedFileTypeException::UnsupportedFileTypeException(std::string const& msg) :
	message(msg)
{
}
img::UnsupportedFileTypeException::UnsupportedFileTypeException(const UnsupportedFileTypeException &original)
	: std::exception(original)
	, message(original.message)
{
}
img::UnsupportedFileTypeException::~UnsupportedFileTypeException() throw ()
{
}
img::UnsupportedFileTypeException& img::UnsupportedFileTypeException::operator=(UnsupportedFileTypeException const& original)
{
	this->message = original.message;
	return *this;
}
const char* img::UnsupportedFileTypeException::what() const throw ()
{
	return message.c_str();
}

img::EasyImage::EasyImage() :
	width(0), height(0), bitmap()
{
}

img::EasyImage::EasyImage(unsigned int _width, unsigned int _height, Color color) :
	width(_width), height(_height), bitmap(width * height, color)
{
}

img::EasyImage::EasyImage(EasyImage const& img) :
	width(img.width), height(img.height), bitmap(img.bitmap)
{
}

img::EasyImage::~EasyImage()
{
	bitmap.clear();
}

img::EasyImage& img::EasyImage::operator=(img::EasyImage const& img)
{
	width = img.width;
	height = img.height;
	bitmap.assign(img.bitmap.begin(), img.bitmap.end());
	return (*this);
}

unsigned int img::EasyImage::get_width() const
{
	return width;
}

unsigned int img::EasyImage::get_height() const
{
	return height;
}

void img::EasyImage::clear(Color color)
{
	for (std::vector<Color>::iterator i = bitmap.begin(); i != bitmap.end(); i++)
	{
		*i = color;
	}
}

Color& img::EasyImage::operator()(unsigned int x, unsigned int y)
{
	assert(x < this->width);
	assert(y < this->height);
	return bitmap.at(x * height + y);
}

Color const& img::EasyImage::operator()(unsigned int x, unsigned int y) const
{
	assert(x < this->width);
	assert(y < this->height);
	return bitmap.at(x * height + y);
}

void img::EasyImage::draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color color)
{
	assert(x0 < this->width && y0 < this->height);
	assert(x1 < this->width && y1 < this->height);
	if (x0 == x1)
	{
		//special case for x0 == x1
		for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
		{
			(*this)(x0, i) = color;
		}
	}
	else if (y0 == y1)
	{
		//special case for y0 == y1
		for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
		{
			(*this)(i, y0) = color;
		}
	}
	else
	{
		if (x0 > x1)
		{
			//flip points if x1>x0: we want x0 to have the lowest value
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		double m = ((double)y1 - (double)y0) / ((double)x1 - (double)x0);
		if (-1.0 <= m && m <= 1.0)
		{
			for (unsigned int i = 0; i <= (x1 - x0); i++)
			{
				(*this)(x0 + i, (unsigned int)roundToInt(y0 + m * i)) = color;
			}
		}
		else if (m > 1.0)
		{
			for (unsigned int i = 0; i <= (y1 - y0); i++)
			{
				(*this)((unsigned int)roundToInt(x0 + (i / m)), y0 + i) = color;
			}
		}
		else if (m < -1.0)
		{
			for (unsigned int i = 0; i <= (y0 - y1); i++)
			{
				(*this)((unsigned int)roundToInt(x0 - (i / m)), y0 - i) = color;
			}
		}
	}
}

//added by Matthew
void img::EasyImage::draw_zbuf_triag(ZBuffer& zbuffer, const Vector3D& A, const Vector3D& B, const Vector3D& C,
	double d, double dx, double dy,
	std::vector<double>& ambientReflection, std::vector<double>& diffuseReflection, std::vector<double>& specularReflection, const double reflectionCoeff,
	std::vector<Light>& lights, bool shadow, Matrix& inversedEyeMatrix,
	EasyImage& texture, unsigned int textureMethod, const std::vector<Vector3D>& surfaceInformation)
{
	assert(zbuffer.zbuffer.size() == this->width);
	assert(zbuffer[0].size() == this->height);
	Point2D projected_A = projectPoint(A, d, dx, dy);
	Point2D projected_B = projectPoint(B, d, dx, dy);
	Point2D projected_C = projectPoint(C, d, dx, dy);

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

	Vector3D normal = Vector3D().vector(w1, w2, w3);
	normal.normalise();

	std::vector<double> ambientColorMultiplier(3);

	for (std::vector<Light>::iterator it_light = lights.begin(); it_light != lights.end(); it_light++)
	{
		for (size_t i = 0; i < 3; i++)
		{
			ambientColorMultiplier[i] += it_light->ambientLight[i] * ambientReflection[i];
		}
	}

	for (unsigned int y_cur = y_min; y_cur <= y_max; y_cur++)
	{
		double x_min = std::numeric_limits<double>::infinity();
		double x_max = -std::numeric_limits<double>::infinity();

		for (unsigned int i = 0; i < 3; i++)
		{
			const Point2D& P = i < 2 ? (i < 1 ? projected_A : projected_B) : projected_C;
			const Point2D& Q = (i + 1) % 3 < 2 ? ((i + 1) % 3 < 1 ? projected_A : projected_B) : projected_C;

			if (P.y == Q.y || (((double)y_cur - P.y) * ((double)y_cur - Q.y)) > 0)
				continue;

			x_min = std::min(x_min, Q.x + ((P.x - Q.x) * (((double)y_cur - Q.y) / (P.y - Q.y))));
			x_max = std::max(x_max, Q.x + ((P.x - Q.x) * (((double)y_cur - Q.y) / (P.y - Q.y))));
		}

		for (int x_cur = roundToInt(x_min + 0.5); x_cur <= roundToInt(x_max - 0.5); x_cur++)
		{
			double z_inv =
				(1.0001 * (grav.z)) + // 1.0001 * 1/zg
				((x_cur - grav.x) * dzdx) + // (x - xg) * dzdx
				((y_cur - grav.y) * dzdy); // (y - yg) * dzdy

			double z_real =
				(grav.z) + // 1/zg
				((x_cur - grav.x) * dzdx) + // (x - xg) * dzdx
				((y_cur - grav.y) * dzdy); // (y - yg) * dzdy

			if (z_inv < zbuffer[x_cur][y_cur])
			{
				std::vector<double> colorMultiplier(ambientColorMultiplier);

				Vector3D currentPixelTo3DPointReal = Vector3D().point(
					((x_cur - dx) * -(1.0 / z_real)) / d, //((xE - dx) * zE) / d
					((y_cur - dy) * -(1.0 / z_real)) / d, //((yE - dx) * zE) / d
					(1.0 / z_real) //-zE
				);

				Vector3D realWorldPoint = currentPixelTo3DPointReal * inversedEyeMatrix;

				for (std::vector<Light>::iterator it_light = lights.begin(); it_light != lights.end(); it_light++)
				{
					if (it_light->specialLight)
					{
						if (shadow && !it_light->isInSight(realWorldPoint))
							continue;

						Vector3D currentPixelVector_FromLight;
						Vector3D currentPixelVector_FromEye = Vector3D().vector(currentPixelTo3DPointReal);

						if (it_light->infinity) currentPixelVector_FromLight = it_light->ldVector;
						else currentPixelVector_FromLight = currentPixelVector_FromEye - Vector3D().vector(it_light->location);

						currentPixelVector_FromLight.normalise();
						currentPixelVector_FromEye.normalise();

						double scalar_cos_alpha = (normal.x * -currentPixelVector_FromLight.x) + (normal.y * -currentPixelVector_FromLight.y) + (normal.z * -currentPixelVector_FromLight.z);
						if (scalar_cos_alpha > 0)
						{
							for (size_t i = 0; i < 3; i++)
							{
								colorMultiplier[i] += it_light->diffuseLight[i] * diffuseReflection[i] * scalar_cos_alpha;
							}
						}

						Vector3D r = ((2 * scalar_cos_alpha) * normal) + currentPixelVector_FromLight;
						r.normalise();

						double scalar_cos_beta = (r.x * -currentPixelVector_FromEye.x) + (r.y * -currentPixelVector_FromEye.y) + (r.z * -currentPixelVector_FromEye.z);
						if (scalar_cos_beta > 0)
						{
							for (size_t i = 0; i < 3; i++)
							{
								colorMultiplier[i] += it_light->specularLight[i] * specularReflection[i] * std::pow(scalar_cos_beta, reflectionCoeff);
							}
						}
					}
				}

				if (texture.get_height() != 0)
				{
					Vector3D p = surfaceInformation[0];
					Vector3D a = surfaceInformation[1];
					Vector3D b = surfaceInformation[2];

					double x_diff = std::abs(realWorldPoint.x - p.x);
					double y_diff = std::abs(realWorldPoint.y - p.y);
					double z_diff = std::abs(realWorldPoint.z - p.z);

					double u;
					double v;

					if (textureMethod == 0)
					{
						double det_x1 = (a.x * b.y) - (a.y * b.x);
						double det_x2 = (a.y * b.z) - (a.z * b.y);
						double det_x3 = (a.x * b.z) - (a.z * b.x);

						if (det_x1 != 0)
						{
							Matrix x1_inv;
							x1_inv(1, 1) = b.y / det_x1;
							x1_inv(1, 2) = -a.y / det_x1;
							x1_inv(2, 1) = -b.x / det_x1;
							x1_inv(2, 2) = a.x / det_x1;

							u = x_diff * x1_inv(1, 1) + y_diff * x1_inv(2, 1);
							v = x_diff * x1_inv(1, 2) + y_diff * x1_inv(2, 2);
						}
						else if (det_x2 != 0)
						{
							Matrix x2_inv;
							x2_inv(1, 1) = b.z / det_x2;
							x2_inv(1, 2) = -a.z / det_x2;
							x2_inv(2, 1) = -b.y / det_x2;
							x2_inv(2, 2) = a.y / det_x2;

							u = y_diff * x2_inv(1, 1) + z_diff * x2_inv(2, 1);
							v = y_diff * x2_inv(1, 2) + z_diff * x2_inv(2, 2);
						}
						else if (det_x3 != 0)
						{
							Matrix x3_inv;
							x3_inv(1, 1) = b.z / det_x3;
							x3_inv(1, 2) = -a.z / det_x3;
							x3_inv(2, 1) = -b.x / det_x3;
							x3_inv(2, 2) = a.x / det_x3;

							u = x_diff * x3_inv(1, 1) + z_diff * x3_inv(2, 1);
							v = x_diff * x3_inv(1, 2) + z_diff * x3_inv(2, 2);
						}
						else continue;
					}
					else if (textureMethod == 1)
					{
						Vector3D c = Vector3D().cross(a, b);

						Matrix x;

						x(1, 1) = a.x;
						x(1, 2) = a.y;
						x(1, 3) = a.z;

						x(2, 1) = b.x;
						x(2, 2) = b.y;
						x(2, 3) = b.z;

						x(3, 1) = c.x;
						x(3, 2) = c.y;
						x(3, 3) = c.z;

						x.inv();

						Vector3D diff = Vector3D().vector(x_diff, y_diff, z_diff);

						Vector3D result = diff * x;

						u = result.x;
						v = result.y;
					}
					else continue;

					u = std::abs(u);
					v = std::abs(v);

					u = u - std::floor(u);
					v = v - std::floor(v);

					Color& texel = texture(roundToInt((double)(texture.get_width() - 1) * u), roundToInt((double)(texture.get_height() - 1) * v));

					(*this)(x_cur, y_cur) = Color(roundToInt(std::min(1.0, colorMultiplier[0]) * texel.red), roundToInt(std::min(1.0, colorMultiplier[1]) * texel.green), roundToInt(std::min(1.0, colorMultiplier[2]) * texel.blue));
				}
				else
					(*this)(x_cur, y_cur) = Color(roundToInt(std::min(1.0, colorMultiplier[0]) * 255), roundToInt(std::min(1.0, colorMultiplier[1]) * 255), roundToInt(std::min(1.0, colorMultiplier[2]) * 255));

				zbuffer[x_cur][y_cur] = z_inv;
			}
		}
	}
}

//added by Matthew
void img::EasyImage::draw_zbuf_line(ZBuffer& zbuffer, unsigned int x0, unsigned int y0, double z0, unsigned int x1, unsigned int y1, double z1, Color color)
{
	assert(x0 < this->width && y0 < this->height);
	assert(x1 < this->width && y1 < this->height);
	assert(zbuffer.zbuffer.size() == this->width);
	assert(zbuffer[0].size() == this->height);
	if (x0 == x1)
	{
		unsigned int pixelcount = std::max(y0, y1) - std::min(y0, y1);
		unsigned int pixel = pixelcount + 1;
		for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
		{
			pixel--;
			double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

			if (newz < zbuffer[x0][i])
			{
				zbuffer[x0][i] = newz;
				(*this)(x0, i) = color;
			}
		}
		assert(pixel == 0);
	}
	else if (y0 == y1)
	{
		unsigned int pixelcount = std::max(x0, x1) - std::min(x0, x1);
		unsigned int pixel = pixelcount + 1;
		for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
		{
			pixel--;
			double newz = (((double)pixel / (double)pixelcount) / z0) + ((1.0 - ((double)pixel / (double)pixelcount)) / z1);

			if (newz < zbuffer[i][y0])
			{
				zbuffer[i][y0] = newz;
				(*this)(i, y0) = color;
			}
		}
		assert(pixel == 0);
	}
	else
	{
		if (x0 > x1)
		{
			//flip points if x1>x0: we want x0 to have the lowest value
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
		}
		double m = ((double)y1 - (double)y0) / ((double)x1 - (double)x0);
		if (-1.0 <= m && m <= 1.0)
		{
			unsigned int pixelcount = (x1 - x0);
			unsigned int pixel = pixelcount + 1;
			for (unsigned int i = 0; i <= (x1 - x0); i++)
			{
				pixel--;
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[x0 + i][(unsigned int)roundToInt(y0 + m * i)])
				{
					zbuffer[x0 + i][(unsigned int)roundToInt(y0 + m * i)] = newz;
					(*this)(x0 + i, (unsigned int)roundToInt(y0 + m * i)) = color;
				}
			}
			assert(pixel == 0);
		}
		else if (m > 1.0)
		{
			unsigned int pixelcount = (y1 - y0);
			unsigned int pixel = pixelcount + 1;
			for (unsigned int i = 0; i <= (y1 - y0); i++)
			{
				pixel--;
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[(unsigned int)roundToInt(x0 + (i / m))][y0 + i])
				{
					zbuffer[(unsigned int)roundToInt(x0 + (i / m))][y0 + i] = newz;
					(*this)((unsigned int)roundToInt(x0 + (i / m)), y0 + i) = color;
				}
			}
			assert(pixel == 0);
		}
		else if (m < -1.0)
		{
			unsigned int pixelcount = (y0 - y1);
			unsigned int pixel = pixelcount + 1;
			for (unsigned int i = 0; i <= (y0 - y1); i++)
			{
				pixel--;
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[(unsigned int)roundToInt(x0 - (i / m))][y0 - i])
				{
					zbuffer[(unsigned int)roundToInt(x0 - (i / m))][y0 - i] = newz;
					(*this)((unsigned int)roundToInt(x0 - (i / m)), y0 - i) = color;
				}
			}
			assert(pixel == 0);
		}
	}
}

std::ostream& img::operator<<(std::ostream& out, EasyImage const& image)
{

	//temporaryily enable exceptions on output stream
	enable_exceptions(out, std::ios::badbit | std::ios::failbit);
	//declare some struct-vars we're going to need:
	bmpfile_magic magic;
	bmpfile_header file_header;
	bmp_header header;
	uint8_t padding[] =
	{ 0, 0, 0, 0 };
	//calculate the total size of the pixel data
	unsigned int line_width = image.get_width() * 3; //3 bytes per pixel
	unsigned int line_padding = 0;
	if (line_width % 4 != 0)
	{
		line_padding = 4 - (line_width % 4);
	}
	//lines must be aligned to a multiple of 4 bytes
	line_width += line_padding;
	unsigned int pixel_size = image.get_height() * line_width;

	//start filling the headers
	magic.magic[0] = 'B';
	magic.magic[1] = 'M';

	file_header.file_size = to_little_endian(pixel_size + sizeof(file_header) + sizeof(header) + sizeof(magic));
	file_header.bmp_offset = to_little_endian(sizeof(file_header) + sizeof(header) + sizeof(magic));
	file_header.reserved_1 = 0;
	file_header.reserved_2 = 0;
	header.header_size = to_little_endian(sizeof(header));
	header.width = to_little_endian(image.get_width());
	header.height = to_little_endian(image.get_height());
	header.nplanes = to_little_endian(1);
	header.bits_per_pixel = to_little_endian(24);//3bytes or 24 bits per pixel
	header.compress_type = 0; //no compression
	header.pixel_size = pixel_size;
	header.hres = to_little_endian(11811); //11811 pixels/meter or 300dpi
	header.vres = to_little_endian(11811); //11811 pixels/meter or 300dpi
	header.ncolors = 0; //no color palette
	header.nimpcolors = 0;//no important colors

	//okay that should be all the header stuff: let's write it to the stream
	out.write((char*)&magic, sizeof(magic));
	out.write((char*)&file_header, sizeof(file_header));
	out.write((char*)&header, sizeof(header));

	//okay let's write the pixels themselves:
	//they are arranged left->right, bottom->top, b,g,r
	for (unsigned int i = 0; i < image.get_height(); i++)
	{
		//loop over all lines
		for (unsigned int j = 0; j < image.get_width(); j++)
		{
			//loop over all pixels in a line
			//we cast &color to char*. since the color fields are ordered blue,green,red they should be written automatically
			//in the right order
			out.write((char*)&image(j, i), 3 * sizeof(uint8_t));
		}
		if (line_padding > 0)
			out.write((char*)padding, line_padding);
	}
	//okay we should be done
	return out;
}
std::istream& img::operator>>(std::istream& in, EasyImage & image)
{
	enable_exceptions(in, std::ios::badbit | std::ios::failbit);
	//declare some struct-vars we're going to need
	bmpfile_magic magic;
	bmpfile_header file_header;
	bmp_header header;
	//a temp buffer for reading the padding at the end of each line
	uint8_t padding[] =
	{ 0, 0, 0, 0 };

	//read the headers && do some sanity checks
	in.read((char*)&magic, sizeof(magic));
	if (magic.magic[0] != 'B' || magic.magic[1] != 'M')
		throw UnsupportedFileTypeException("Could not parse BMP File: invalid magic header");
	in.read((char*)&file_header, sizeof(file_header));
	in.read((char*)&header, sizeof(header));
	if (le32toh(header.pixel_size) + le32toh(file_header.bmp_offset) != le32toh(file_header.file_size))
		throw UnsupportedFileTypeException("Could not parse BMP File: file size mismatch");
	if (le32toh(header.header_size) != sizeof(header))
		throw UnsupportedFileTypeException("Could not parse BMP File: Unsupported BITMAPV5HEADER size");
	if (le32toh(header.compress_type) != 0)
		throw UnsupportedFileTypeException("Could not parse BMP File: Only uncompressed BMP files can be parsed");
	if (le32toh(header.nplanes) != 1)
		throw UnsupportedFileTypeException("Could not parse BMP File: Only one plane should exist in the BMP file");
	if (le32toh(header.bits_per_pixel) != 24)
		throw UnsupportedFileTypeException("Could not parse BMP File: Only 24bit/pixel BMP's are supported");
	//if height<0 -> read top to bottom instead of bottom to top
	bool invertedLines = from_little_endian(header.height) < 0;
	image.height = std::abs(from_little_endian(header.height));
	image.width = std::abs(from_little_endian(header.width));
	unsigned int line_padding = from_little_endian(header.pixel_size) / image.height - (3 * image.width);
	//re-initialize the image bitmap
	image.bitmap.clear();
	image.bitmap.assign(image.height * image.width, Color());
	//okay let's read the pixels themselves:
	//they are arranged left->right., bottom->top if height>0, top->bottom if height<0, b,g,r
	for (unsigned int i = 0; i < image.get_height(); i++)
	{
		//loop over all lines
		for (unsigned int j = 0; j < image.get_width(); j++)
		{
			//loop over all pixels in a line
			//we cast &color to char*. since the color fields are ordered blue,green,red, the data read should be written in the right variables
			if (invertedLines)
			{
				//store top-to-bottom
				in.read((char*)&image(j, image.height - 1 - i), 3 * sizeof(uint8_t));
			}
			else
			{
				//store bottom-to-top
				in.read((char*)&image(j, i), 3 * sizeof(uint8_t));
			}
		}
		if (line_padding > 0)
		{
			in.read((char*)padding, line_padding);
		}
	}
	//okay we're done
	return in;
}
