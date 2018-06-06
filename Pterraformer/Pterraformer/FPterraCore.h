#pragma once

#include <GLFW/glfw3.h>
#include "ICamera.h"
#include "FBasicRenderer.h"

class FPterraCore
{
public:
	FPterraCore(int initialWidth, int initialHeight);
	~FPterraCore();

	bool init();
	void close();
	void start();
protected:
private:
	static void setFramebufferCallback(GLFWwindow* window, int width, int height);
	// IMGUI will attempt to steal input focus. Ensure that this method, or the camera process method is called when mouse movement is needed
	void processInput(GLFWwindow* window);
private:
	int screenWidth;
	int screenHeight;

	bool quit;

	GLFWwindow* window;
	ICamera* camera;
	FBasicRenderer* renderer;
	static bool FramebufferSizeChanged;
};

