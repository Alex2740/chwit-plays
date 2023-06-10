#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>


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

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

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

GLfloat functionColorBackgound(float x, int stepShit)
{
	GLfloat step = 1.0f / 6.0f;
	GLfloat new_x = x - stepShit * step;

	if (new_x < 0) new_x += 1;
	if (new_x > 1) new_x -= 1;

	if (new_x <= step) return new_x / step;
	if (new_x <= 3 * step) return 1.0f;
	if (new_x <= 4 * step) return -new_x / step + 4.0f;
	return 0.0f;
}

void setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glClearColor(blue, green, red, alpha);
}

void draw(GLFWwindow* window, GLuint shaderProgram, GLuint VAO)
{
	GLfloat x = ((GLfloat) frame_id) / ((GLfloat) frameNumber);

	// Specify the color of the background
	setBackgroundColor(functionColorBackgound(x, -2), functionColorBackgound(x, 0), functionColorBackgound(x, 2), 1.0f);

	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	// Tell OpenGL which Shader Program we want to use
	glUseProgram(shaderProgram);

	// Bind the VAO so OpenGL knows to use it
	glBindVertexArray(VAO);

	// Draw the triangle using the GL_TRIANGLES primitive
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
}

void renderFrame(GLFWwindow* window, GLuint shaderProgram, GLuint VAO)
{
	draw(window, shaderProgram, VAO);
	glFlush();
}

void captureFrame(GLFWwindow* window, GLuint shaderProgram, GLuint VAO)
{
	if (++frame_id % 100 == 0)
		std::cerr << "frame " << frame_id << std::endl;

	// Attacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);

	// Appeler la fonction de rendu
	renderFrame(window, shaderProgram, VAO);

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

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialisation du FBO
	initFBO();

	// Main while loop
	while (!glfwWindowShouldClose(window) && !hasFinishVideo)
	{
		captureFrame(window, shaderProgram, VAO);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

	// Release the video for reading
	video.release();

	return 0;
}