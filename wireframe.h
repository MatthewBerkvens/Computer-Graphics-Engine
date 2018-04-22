//
// Created by matthew
//

#ifndef ENGINE_WIREFRAME_H
#define ENGINE_WIREFRAME_H
#include "easy_image.h"
#include "lib3d.h"
#include "ini_configuration.h"
#include "bodies.h"
#include "lib_lsystem.h"
#include <assert.h>

namespace wireframe {
	img::EasyImage generate_Wireframe(const ini::Configuration& conf, bool UseZBuffering);

	void parseLineDrawing(lib3d::Figure& figure, const ini::Configuration& conf, std::string& name);
}
#endif //ENGINE_WIREFRAME_H