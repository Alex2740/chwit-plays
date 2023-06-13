#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>
#include <opencv2/opencv.hpp>

GLuint fbo;     // Identifiant du FBO
GLuint texture; // Identifiant de la texture attachée au FBO

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y = 0;
double rotate_x = 0;

const int fps = 25;
int frame_id = 0;

const int width = 800;
const int height = 600;

bool shouldExit = false;

uint8_t *framebuffer = new uint8_t[width * height * 3];

cv::VideoWriter video;

void saveImage()
{
    cv::Mat image(height, width, CV_8UC3, framebuffer);
    cv::imwrite("screenshot.png", image);
    std::cout << "Image sauvegardée : screenshot.png" << std::endl;
}

void keyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
        std::cout << "sauvegarde" << std::endl;
        saveImage();
        break;
    }
}

void initFBO()
{
    // Créer le FBO
    std::cout << "fbo-------------" << std::endl;
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

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys(int key, int x, int y)
{
    std::cerr << "key pressed: " << key << std::endl;

    //  Right arrow - increase rotation by 5 degree
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        rotate_y += 5;
        break;
    case GLUT_KEY_LEFT:
        rotate_y -= 5;
        break;
    case GLUT_KEY_UP:
        rotate_x += 5;
        break;
    case GLUT_KEY_DOWN:
        rotate_x -= 5;
        break;
    case GLUT_KEY_END:
        shouldExit = true;
        std::cerr << "termine" << std::endl;
        break;
    }

    //  Request display update
    glutPostRedisplay();
}

void draw()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Rotate when user changes rotate_x and rotate_y
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    // Multi-colored side - FRONT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5); // P1 is red

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5); // P2 is green

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5); // P3 is blue

    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, -0.5); // P4 is purple

    glEnd();
}

void renderFunction()
{
    draw();
    glFlush();
    // glutSwapBuffers(); // sauf si     glutInitDisplayMode(GLUT_DOUBLE);
}

void captureFunction(int value)
{
    if (++frame_id % 10 == 0)
        std::cerr << "frame " << frame_id << std::endl;

    // Attacher le FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);

    // Appeler la fonction de rendu
    renderFunction();

    // Lire les données de la texture attachée au FBO
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);

    // Convertir le tableau framebuffer en une image OpenCV
    cv::Mat image(height, width, CV_8UC3, framebuffer);

    // Détacher le FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Écrire l'image dans la vidéo
    video.write(image);

    glutTimerFunc(fps, captureFunction, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(width, height);

    // int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    int fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
    cv::Size size = cv::Size(width, height);

    video = cv::VideoWriter("output.avi", fourcc, fps, size);

    if (!video.isOpened())
    {
        std::cerr << "Erreur lors de l'ouverture du fichier vidéo" << std::endl;
        return 1;
    }

    glutCreateWindow("OpenGL - First window demo");
    // glutHideWindow();
    glutDisplayFunc(renderFunction);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboardFunc);
    glutTimerFunc(0, captureFunction, 0);
    glewInit();
    initFBO();
    glutMainLoop();
    video.release();

    return 0;
}
