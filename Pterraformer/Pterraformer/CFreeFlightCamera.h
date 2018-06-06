#pragma once
#include "ICamera.h"
#include <glm/glm.hpp>

class CFreeFlightCamera : public ICamera
{
public:
	CFreeFlightCamera(float initialVelocity, glm::vec3 initialMouseSensitivity);
	~CFreeFlightCamera();

	void Move(CameraDirection direction);
	void MouseLook(glm::vec2 newPosition);
	void ChangePitch(float degrees);
	void ChangeYaw(float degress);
	void ProcessInput(GLFWwindow* window);
private:
	bool enableMouseMove;
	glm::vec2 mousePosition;
};

