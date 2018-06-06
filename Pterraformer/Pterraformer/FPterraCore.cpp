#include "stdafx.h"
#include "FPterraCore.h"
#include "CTerrain.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>

#include "imgui.h"
#include "CFreeFlightCamera.h"

bool FPterraCore::FramebufferSizeChanged = false;

FPterraCore::FPterraCore(int initialWidth, int initialHeight)
	: screenWidth(initialWidth), screenHeight(initialHeight)
{
	camera = nullptr;
}


FPterraCore::~FPterraCore()
{
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
}

void FPterraCore::close()
{
	glfwTerminate();
	window = nullptr;
}

bool FPterraCore::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(screenWidth, screenHeight, "Pterraformer", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	setFramebufferCallback(nullptr, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, &setFramebufferCallback);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	camera = new CFreeFlightCamera(1.f, glm::vec3(0.001f, 0.001f, 0.0f));
	camera->SetPosition(glm::vec3(0.f, 1000.f, -50.f));
	camera->LookAt(glm::vec3(0.f, 600.f, 100.f));
	camera->SetClipping(1.f, 100000000.f);
	camera->SetFOV(45.f);
	camera->SetViewport(0, 0, screenWidth, screenHeight);

	//IMGUI_CHECKVERSION();
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Let's leave the GUI out for now and get the mesh rendering
	// TODO : Add the GUI for the toolkit

	// Setup style
	//ImGui::StyleColorsDark();

	return true;
}

void FPterraCore::setFramebufferCallback(GLFWwindow* window, int width, int height)
{
	FramebufferSizeChanged = true;
}

void FPterraCore::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		renderer->SetMode(GL_POINTS);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		renderer->SetMode(GL_TRIANGLES);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		renderer->SetMode(GL_LINES);

	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		camera->ModifySpeed(.1f);
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		camera->ModifySpeed(-.1f);

	camera->ProcessInput(window);
}

void FPterraCore::start()
{
	// Shit code but good enough until it's all working

	renderer = new FBasicRenderer(camera);
	FHeightmap* heightmap = new FHeightmap(TempVars::TerrainFile());
	CTerrain* terrain = new CTerrain(heightmap);

	std::vector<CMesh>* vaos = new std::vector<CMesh>();
	vaos->push_back(*terrain->Mesh);
	double previousTime = glfwGetTime();
	int frameCount = 0;
	while (!glfwWindowShouldClose(window))
	{
		if (FramebufferSizeChanged)
		{
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			camera->SetViewport(0, 0, w, h);
		}

		processInput(window);

		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		int x, y, width, height;
		camera->GetViewport(x, y, width, height);
		glViewport(x, y, width, height);
		camera->UpdateView();

		renderer->Render(vaos);

		glfwSwapBuffers(window);
		glfwPollEvents();
		frameCount++;
		double currentTime = glfwGetTime();
		// Replace with actual FPS counter when UX is in
		if (currentTime - previousTime >= 1.0f)
		{
			std::cout << frameCount << " fps" << std::endl;
			frameCount = 0;
			previousTime = currentTime;
		}
	}

	delete vaos;
	delete terrain;
	delete heightmap;
	delete renderer;
}