#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include "api.h"

float video_minutes = 0.5f;


GLuint fbo;     // Identifiant du FBO
GLuint texture; // Identifiant de la texture attachée au FBO

const int fps = 25;
int frame_id = 0;

const int width = 800;
const int height = 600;

uint8_t* framebuffer = new uint8_t[width * height * 3];

cv::VideoWriter video;

int frameNumber = video_minutes * 60 * fps;
bool hasFinishVideo = false;

// Vertices coordinates
GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS (BGR)           //
	-0.5f, 0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.02f, 0.3f,  0.8f, // Lower left corner
	 0.5f, 0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.02f, 0.3f,  0.8f, // Lower right corner
	 0.0f,  -0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     0.32f, 0.6f,  1.0f, // Upper corner
	-0.25f, -0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.17f, 0.45f, 0.9f, // Inner left
	 0.25f, -0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.17f, 0.45f, 0.9f, // Inner right
	 0.0f, 0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.02f, 0.3f,  0.8f  // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

void initFBO()
{
	// Créer le FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Créer la texture attachée au FBO
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attacher la texture au FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// Vérifier l'état du FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Erreur lors de la création du FBO" << std::endl;
		// Gérer l'erreur ici
	}

	// Détacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void draw(GLFWwindow* window, Shader shaderProgram, GLuint uniID, VAO vao)
{
	GLfloat x = ((GLfloat)frame_id) / ((GLfloat)frameNumber);

	// Specify the color of the background
	glClearColor(Api::functionColorBackgound(x, 2), Api::functionColorBackgound(x, 0), Api::functionColorBackgound(x, -2), 1.0f);

	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Tell OpenGL which Shader Program we want to use
	shaderProgram.Activate();
	// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
	glUniform1f(uniID, 0.5f);
	// Bind the VAO so OpenGL knows to use it
	vao.Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
}

void renderFrame(GLFWwindow* window, Shader shaderProgram, GLuint uniID, VAO vao)
{
	draw(window, shaderProgram, uniID, vao);
	glFlush();
}

void captureFrame(GLFWwindow* window, Shader shaderProgram, GLuint uniID, VAO vao)
{
	if (++frame_id % 100 == 0)
		std::cerr << "frame " << frame_id << std::endl;

	// Attacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);

	// Appeler la fonction de rendu
	renderFrame(window, shaderProgram, uniID, vao);

	// Lire les données de la texture attachée au FBO
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);

	// Convertir le tableau framebuffer en une image OpenCV
	cv::Mat image(height, width, CV_8UC3, framebuffer);

	// Détacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Écrire l'image dans la vidéo
	video.write(image);

	if (frame_id >= frameNumber) hasFinishVideo = true;
}

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

	// Create video where images will be saved
	video = cv::VideoWriter("output.avi", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, cv::Size(width, height));

	// Error check if the video fails to open
	if (!video.isOpened())
	{
		std::cerr << "Erreur lors de l'ouverture du fichier vidéo" << std::endl;
		return 1;
	}

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(height, width, "OpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 800);

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Initialisation du FBO
	initFBO();

	// Main while loop
	while (!glfwWindowShouldClose(window) && !hasFinishVideo)
	{
		captureFrame(window, shaderProgram, uniID, VAO1);
		
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	// Release the video for reading
	video.release();


	return 0;
}