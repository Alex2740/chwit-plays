#pragma once

#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "../Objects/Object.h"
#include "../Objects/Cube.h"

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