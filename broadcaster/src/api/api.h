#pragma once

#include <iostream>
#include <cpr/cpr.h>
#include <api/json.hpp>
using json = nlohmann::json;

#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

#include "objects/object.h"
#include "objects/cube.h"


namespace API
{
	class Api
	{
	public:

		void static UpdateObjects(Graphics::Camera* Camera, std::array<Object*, 10> Objects, Graphics::Renderer* Renderer, Graphics::Texture* Texture);

	};
}