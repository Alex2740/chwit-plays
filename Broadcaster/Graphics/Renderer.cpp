#include "renderer.h"


void Graphics::Renderer::RenderCube(Cube* Cube, Shader ShaderProgram)
{
	VertexArrayObject.Bind();

	// Vertices coordinates
	GLfloat Vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	for (int i = 0; i < 36; i++)
	{
		int Index = i * 8;

		Vertices[Index] *= Cube->HalfSizeX;
		Vertices[Index + 1] *= Cube->HalfSizeY;
		Vertices[Index + 2] *= Cube->HalfSizeZ;
	}

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(Vertices, sizeof(Vertices));

	// Indices for vertices order
	GLuint Indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	// Generates Element Buffer Object and links it to indices
	EBO EBO(Indices, sizeof(Indices));

	// Links VBO attributes such as coordinates and colors to vertexArrayObject
	VertexArrayObject.LinkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VertexArrayObject.LinkAttrib(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VertexArrayObject.LinkAttrib(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glm::mat4 Model = glm::mat4(1.0f);
	glm::vec3 tmp(Cube->X, Cube->Y, Cube->Y);
	Model = glm::translate(Model, tmp);
	glm::vec3 Rotation(Cube->RotationX, Cube->RotationY, Cube->RotationZ);

	// Rotation on Z-Axis
	Model = glm::rotate(Model, -Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // PSEUDO FIX

	// Rotation on Y-Axis
	Model = glm::rotate(Model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	// Rotation on X-Axis
	Model = glm::rotate(Model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE, &Model[0][0]);
	TextureMap[Cube]->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	VertexArrayObject.Unbind();

	VBO.Delete();
	EBO.Delete();
}


Graphics::Renderer::Renderer()
{
	// Generates Vertex Array Object and binds it
	VertexArrayObject = VAO();
	VertexArrayObject.Bind();

	// Generate Texture Map
	TextureMap = std::map<Object*, Texture*>();
}


void Graphics::Renderer::Delete()
{
	// Delete all the objects we've created
	VertexArrayObject.Delete();
}


void Graphics::Renderer::RenderObject(Object* Object, Shader ShaderProgram)
{
	switch (Object->GetType())
	{
	case ObjectCube:
		RenderCube(dynamic_cast<Cube*>(Object), ShaderProgram);
		break;
	default:
		break;
	}
}


void Graphics::Renderer::BindTexture(Object* Object, Texture* Texture)
{
	TextureMap[Object] = Texture;
}
