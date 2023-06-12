#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include "Graphics/Texture.h"
#include "Graphics/shaderClass.h"
#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"

#include "Objects/Cube.h"


const unsigned int width = 800;
const unsigned int height = 800;


int main()
{
	Cube MyCube1 = Cube(0, 0, 0, 0, 0, 0, 1, 1, 1);
	Cube MyCube2 = Cube(5, 5, 5, 0, 0, 0, 2, 2, 2);

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Graphics::Shader ShaderProgram("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
	glm::mat4 Model = glm::identity<glm::mat4>();
	ShaderProgram.setMat4("model", Model);

	// Create the Renderer
	Graphics::Renderer Renderer = Graphics::Renderer();

	// Create the Texture
	Graphics::Texture brickTex("Resources/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(ShaderProgram, "tex0", 0);

	// Bind Texture to Object
	Renderer.BindTexture(&MyCube1, &brickTex);
	Renderer.BindTexture(&MyCube2, &brickTex);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Graphics::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		ShaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);

		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, ShaderProgram, "camMatrix");

		Renderer.RenderObject(&MyCube1, ShaderProgram);
		Renderer.RenderObject(&MyCube2, ShaderProgram);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	Renderer.Delete();
	brickTex.Delete();
	ShaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}