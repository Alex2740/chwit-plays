#include "capture/capturer.h"

Capture::Capturer::Capturer(int Height, int Width, int Fps, int MaxFrame)
{
	this->Height = Height;
	this->Width = Width;
	this->Fps = Fps;
	this->FrameBuffer = new uint8_t[Width * Height * 3];
	this->FBO;
	this->Texture;
	this->MaxFrame = MaxFrame;
	
	// Create video where images will be saved
	Video = cv::VideoWriter("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), Fps, cv::Size(Width, Height));

	Init();
}

void Capture::Capturer::Init()
{
	// Cr�er le FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Cr�er la texture attach�e au FBO
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// V�rifier l'�tat du FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Erreur lors de la cr�ation du FBO" << std::endl;
		// G�rer l'erreur ici
	}

	// D�tacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Capture::Capturer::CaptureFrame()
{
	if (++FrameId % 100 == 0)
		std::cerr << "frame " << FrameId << std::endl;

	if (MaxFrame >= 0 && FrameId >= MaxFrame) HasReachMaxFrame = true;

	// Attacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, Width, Height);

	// Appeler la fonction de rendu
	DrawFrame();
	glFlush();

	// Lire les donn�es de la texture attach�e au FBO
	glBindTexture(GL_TEXTURE_2D, Texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, FrameBuffer);
	//glReadPixels(0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, )

	// Convertir le tableau framebuffer en une image OpenCV
	cv::Mat image(Height, Width, CV_8UC3, FrameBuffer);

	// D�tacher le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// �crire l'image dans la vid�o
	//Video.write(image);

	//output.write(reinterpret_cast<char*>(FrameBuffer));

	//std::fwrite(image.data, 1, image.size, stdout);
	//std::fflush(stdout);

	fwrite(FrameBuffer, Width*Height*3, 1, stdout);
	fflush(stdout);

	//std::string out((char*)image.data, image.total() * image.elemSize());
	//std::cout << out;
}

void Capture::Capturer::Delete()
{
	Video.release();
}
