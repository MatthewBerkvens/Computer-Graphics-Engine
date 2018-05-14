//
// Created by matthew
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

#include "mylibrary.h"

#include <stdexcept>
#include <iomanip>
#include <vector>

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

#endif