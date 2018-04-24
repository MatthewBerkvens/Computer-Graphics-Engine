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
#include "mylibrary.h"
#include "lib3d.h"
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <limits>

#define le32toh(x) (x)

namespace
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

img::Color::Color() :
	blue(0), green(0), red(0)
{
}
img::Color::Color(uint8_t r, uint8_t g, uint8_t b) :
	blue(b), green(g), red(r)
{
}
img::Color::~Color()
{
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

img::Color& img::EasyImage::operator()(unsigned int x, unsigned int y)
{
	assert(x < this->width);
	assert(y < this->height);
	return bitmap.at(x * height + y);
}

img::Color const& img::EasyImage::operator()(unsigned int x, unsigned int y) const
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
img::ZBuffer::ZBuffer(const unsigned int width, const unsigned int height) {
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

//added by Matthew
void img::EasyImage::draw_zbuf_triag(ZBuffer& zbuffer, const Vector3D& A, const Vector3D& B, const Vector3D& C, double d, double dx, double dy, const Color& color)
{
	assert(zbuffer.size() == this->width);
	assert(zbuffer[0].size() == this->height);
	Point2D projected_A = Point2D(((d * A.x) / -A.z) + dx, ((d * A.y) / -A.z) + dy);
	Point2D projected_B = Point2D(((d * B.x) / -B.z) + dx, ((d * B.y) / -B.z) + dy);
	Point2D projected_C = Point2D(((d * C.x) / -C.z) + dx, ((d * C.y) / -C.z) + dy);

	unsigned int y_min = roundToInt(std::min(projected_A.y, std::min(projected_B.y, projected_C.y)) + 0.5);
	unsigned int y_max = roundToInt(std::max(projected_A.y, std::max(projected_B.y, projected_C.y)) - 0.5);

	for (unsigned int y_cur = y_min; y_cur < y_max; y_cur++)
	{
		double x_min = std::numeric_limits<double>::infinity();
		double x_max = -std::numeric_limits<double>::infinity();

		for (unsigned int i = 0; i < 3; i++)
		{
			const Point2D& P = i < 2 ? (i < 1 ? projected_A : projected_B) : projected_C;
			const Point2D& Q = (i + 1) % 3 < 2 ? ((i + 1) % 3 < 1 ? projected_A : projected_B) : projected_C;

			if (P.y == Q.y || ((y_cur - P.y) * (y_cur - Q.y)) > 0) continue;

			x_min = std::min(x_min, Q.x + ((P.x - Q.x) * ((y_cur - Q.y) / (P.y - Q.y))));
			x_max = std::max(x_max, Q.x + ((P.x - Q.x) * ((y_cur - Q.y) / (P.y - Q.y))));
		}

		for (int x_cur = roundToInt(x_min + 0.5); x_cur <= roundToInt(x_max - 0.5); x_cur++)
		{
			double w1 = ((B.y - A.y) * (C.z - A.z)) - ((B.z - A.z) * (C.y - A.y));

			double w2 = ((B.z - A.z) * (C.x - A.x)) - ((B.x - A.x) * (C.z - A.z));

			double w3 = ((B.x - A.x) * (C.y - A.y)) - ((B.y - A.y) * (C.x - A.x));

			double k = w1 * A.x + w2 * A.y + w3 * A.z;

			double dzdx = w1 / (-d * k);
			double dzdy = w2 / (-d * k);

			double z_inv =
				(1.0001 * (1 / (3 * A.z) + 1 / (3 * B.z) + 1 / (3 * C.z))) + // 1/zg
				((x_cur - ((projected_A.x + projected_B.x + projected_C.x) / 3)) * dzdx) + // (x - xg) * dzdx
				((y_cur - ((projected_A.y + projected_B.y + projected_C.y) / 3)) * dzdy); // (x - xg) * dzdy
			
			if (z_inv < zbuffer[x_cur][y_cur])
			{
				(*this)(x_cur, y_cur) = color;
				zbuffer[x_cur][y_cur] = z_inv;
			}
		}
	}
}

//added by Matthew
void img::EasyImage::draw_zbuf_line(ZBuffer& zbuffer, unsigned int x0, unsigned int y0, double z0, unsigned int x1, unsigned int y1, double z1, const Color& color)
{
	assert(x0 < this->width && y0 < this->height);
	assert(x1 < this->width && y1 < this->height);
	assert(zbuffer.size() == this->width);
	assert(zbuffer[0].size() == this->height);
	if (x0 == x1)
	{
		unsigned int pixelcount = std::max(y0, y1) - std::min(y0, y1) + 1;
		unsigned int pixel = pixelcount;
		for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
		{
			double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

			if (newz < zbuffer[x0][i])
			{
				zbuffer[x0][i] = newz;
				(*this)(x0, i) = color;
			}
			pixel--;
		}
		assert(pixel == 0);
	}
	else if (y0 == y1)
	{
		unsigned int pixelcount = std::max(x0, x1) - std::min(x0, x1) + 1;
		unsigned int pixel = pixelcount;
		for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
		{
			double newz = (((double)pixel / (double)pixelcount) / z0) + ((1.0 - ((double)pixel / (double)pixelcount)) / z1);

			if (newz < zbuffer[i][y0])
			{
				zbuffer[i][y0] = newz;
				(*this)(i, y0) = color;
			}
			pixel--;
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
			unsigned int pixelcount = (x1 - x0) + 1;
			unsigned int pixel = pixelcount;
			for (unsigned int i = 0; i <= (x1 - x0); i++)
			{
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[x0 + i][(unsigned int)roundToInt(y0 + m * i)])
				{
					zbuffer[x0 + i][(unsigned int)roundToInt(y0 + m * i)] = newz;
					(*this)(x0 + i, (unsigned int)roundToInt(y0 + m * i)) = color;
				}
				pixel--;
			}
			assert(pixel == 0);
		}
		else if (m > 1.0)
		{
			unsigned int pixelcount = (y1 - y0) + 1;
			unsigned int pixel = pixelcount;
			for (unsigned int i = 0; i <= (y1 - y0); i++)
			{
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[(unsigned int)roundToInt(x0 + (i / m))][y0 + i])
				{
					zbuffer[(unsigned int)roundToInt(x0 + (i / m))][y0 + i] = newz;
					(*this)((unsigned int)roundToInt(x0 + (i / m)), y0 + i) = color;
				}
				pixel--;
			}
			assert(pixel == 0);
		}
		else if (m < -1.0)
		{
			unsigned int pixelcount = (y0 - y1) + 1;
			unsigned int pixel = pixelcount;
			for (unsigned int i = 0; i <= (y0 - y1); i++)
			{
				double newz = (((double)pixel / (double)pixelcount) / z0) + ((1 - ((double)pixel / (double)pixelcount)) / z1);

				if (newz < zbuffer[(unsigned int)roundToInt(x0 - (i / m))][y0 - i])
				{
					zbuffer[(unsigned int)roundToInt(x0 - (i / m))][y0 - i] = newz;
					(*this)((unsigned int)roundToInt(x0 - (i / m)), y0 - i) = color;
				}
				pixel--;
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
