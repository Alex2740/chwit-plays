#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include "graphics/shaderClass.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"

namespace Capture
{
	class Capturer
	{
	private:
		unsigned int Height;
		unsigned int Width;

		GLuint FBO;
		GLuint Texture;

		int Fps;
		int FrameId = 0;

		uint8_t* FrameBuffer;

		cv::VideoWriter Video;

		void Init();

	public:
		Capturer(int Height, int Width, int Fps);

		void CaptureFrame();

		void virtual DrawFrame() = 0;

		void Delete();
	};
}