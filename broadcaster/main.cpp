#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <list>
#include <unistd.h>

#include "graphics/texture.h"
#include "graphics/shaderClass.h"
#include "graphics/camera.h"
#include "graphics/renderer.h"

#include "objects/object.h"
#include "objects/cube.h"

#include "capture/capturer.h"

#include "api/api.h"

#define MAX_SCENE_OBJECTS 10


const float MaxVideoLength = -1;

const unsigned int Width = 800;
const unsigned int Height = 800;
const int Fps = 25;

int MaxFrame = MaxVideoLength * 60 * Fps;
bool VideoHasFinish = false;

const std::string cameraId;
std::array<Object*, MAX_SCENE_OBJECTS> Objects = std::array<Object*, MAX_SCENE_OBJECTS>();

GLFWwindow* Window;
Graphics::Shader* ShaderProgram;
Graphics::Renderer* Renderer;
Graphics::Camera* Camera;


class MyCapturer : public Capture::Capturer
{
public:
	MyCapturer(int Height, int Width, int Fps, int MaxFrame) : Capturer(Height, Width, Fps, MaxFrame) {};

	void DrawFrame() override
	{
		// Specify the color of the background
		glClearColor(0.17f, 0.13f, 0.07f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		ShaderProgram->Activate();

		// Updates and exports the camera matrix to the Vertex Shader
		Camera->Matrix(45.0f, 0.1f, 100.0f, *ShaderProgram, "camMatrix");

		for (auto Object : Objects)
		{
			if (Object == nullptr) continue;
			Renderer->RenderObject(Object, *ShaderProgram);
		}
	}
};


MyCapturer* Capturer;


int main()
{
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
	Window = glfwCreateWindow(Width, Height, "OpenGL", NULL, NULL);

	// Error check if the window fails to create
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(Window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, Width, Height);
	
	// Generates Shader object using shaders default.vert and default.frag
	ShaderProgram = new Graphics::Shader("resources/shaders/default.vert", "resources/shaders/default.frag");
	glm::mat4 Model = glm::identity<glm::mat4>();
	ShaderProgram->setMat4("model", Model);

	// Create the Renderer
	Renderer = new Graphics::Renderer();

	// Create the Texture
	Graphics::Texture brickTex("resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(*ShaderProgram, "tex0", 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	Cube MyCube1 = Cube(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	Objects[0] = &MyCube1;
	Renderer->BindTexture(&MyCube1, &brickTex);

	// Creates camera object
	Camera = new Graphics::Camera(0, Width, Height, glm::vec3(0.0f, 0.0f, 5.0f));

	Capturer = new MyCapturer(Height, Width, Fps, MaxFrame);

	// Main while loop
	while (!VideoHasFinish && !glfwWindowShouldClose(Window))
	{
		API::Api::UpdateObjects(Camera, Objects, Renderer, &brickTex);

		// Handles camera inputs
		//VideoHasFinish = Camera->Inputs(Window);
		
		Capturer->CaptureFrame();
		Capturer->DrawFrame();

		if (Capturer->HasReachMaxFrame) VideoHasFinish = true;

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(Window);

		// Take care of all GLFW events
		glfwPollEvents();

		sleep(1);
	}

	// Delete all the objects we've created
	Renderer->Delete();
	brickTex.Delete();
	ShaderProgram->Delete();
	Capturer->Delete();

	// Delete window before ending the program
	glfwDestroyWindow(Window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}