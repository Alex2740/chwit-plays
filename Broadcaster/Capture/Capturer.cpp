#include "Capturer.h"

Capture::Capturer::Capturer(int Height, int Width, int Fps)
{
	this->Height = Height;
	this->Width = Width;
	this->Fps = Fps;
	this->FrameBuffer = new uint8_t[Width * Height * 3];
	this->FBO;
	this->Texture;
	
	// Create video where images will be saved
	Video = cv::VideoWriter("output.avi", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), Fps, cv::Size(Width, Height));

	Init();
}

void Capture::Capturer::Init()
{
	// Créer le FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Créer la texture attachée au FBO
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attacher la texture au FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

	// Vérifier l'état du FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Erreur lors de la création du FBO" << std::endl;
		// Gérer l'erreur ici
	}

	// Détacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Capture::Capturer::CaptureFrame()
{
	if (++FrameId % 100 == 0)
		std::cerr << "frame " << FrameId << std::endl;

	// Attacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, Width, Height);

	// Appeler la fonction de rendu
	DrawFrame();
	glFlush();

	// Lire les données de la texture attachée au FBO
	glBindTexture(GL_TEXTURE_2D, Texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, FrameBuffer);

	// Convertir le tableau framebuffer en une image OpenCV
	cv::Mat image(Height, Width, CV_8UC3, FrameBuffer);

	// Détacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Écrire l'image dans la vidéo
	Video.write(image);
}
