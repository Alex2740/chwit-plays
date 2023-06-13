#pragma once

#include "graphics/texture.h"
#include "graphics/VAO.h"
#include "graphics/VBO.h"
#include "graphics/EBO.h"

#include "objects/object.h"
#include "objects/cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>


namespace Graphics
{
	class Renderer
	{
	private:
		VAO VertexArrayObject;
		std::map<Object*, Texture*> TextureMap;

		void RenderCube(Cube* Cube, Shader ShaderProgram);
	public:
		Renderer();

		void Delete();

		void RenderObject(Object* Object, Shader ShaderProgram);
		
		void BindTexture(Object* Object, Texture* Texture);
	};
};