#include "CFreeFlightCamera.h"



CFreeFlightCamera::CFreeFlightCamera(float initialVelocity, glm::vec3 initialMouseSensitivity)
	: ICamera(initialVelocity, initialMouseSensitivity, FREE)
{
}


CFreeFlightCamera::~CFreeFlightCamera()
{
}

void CFreeFlightCamera::Move(CameraDirection direction)
{
	unsigned long ulDirection = direction.to_ulong();
	if ((ulDirection & CAMERA_UP) == CAMERA_UP)
		positionDelta += cameraUp * cameraVelocity;
	if ((ulDirection & CAMERA_DOWN) == CAMERA_DOWN)
		positionDelta -= cameraUp * cameraVelocity;
	if ((ulDirection & CAMERA_LEFT) == CAMERA_LEFT)
		positionDelta -= glm::cross(this->direction, cameraUp) * cameraVelocity;
	if ((ulDirection & CAMERA_RIGHT) == CAMERA_RIGHT)
		positionDelta += glm::cross(this->direction, cameraUp) * cameraVelocity;
	if ((ulDirection & CAMERA_FORWARD) == CAMERA_FORWARD)
		positionDelta += this->direction * cameraVelocity;
	if ((ulDirection & CAMERA_BACK) == CAMERA_BACK)
		positionDelta -= this->direction * cameraVelocity;
}

void CFreeFlightCamera::ChangePitch(float degrees)
{
	cameraPitch += degrees;

	// Check Gimbal bounds
	if (cameraPitch > 359.9f)
		cameraPitch -= 359.9f;
	else if (cameraPitch < -359.9f)
		cameraPitch += 359.9f;
}

void CFreeFlightCamera::ChangeYaw(float degrees)
{
	if (cameraPitch > 90 && cameraPitch < 270 || (cameraPitch < -90 && cameraPitch > -270))
		cameraYaw -= degrees;
	else
		cameraYaw += degrees;

	if (cameraYaw > 359.9f)
		cameraYaw -= 359.9f;
	else if (cameraYaw < -359.9f)
		cameraYaw += 359.9f;
}

void CFreeFlightCamera::ProcessInput(GLFWwindow* window)
{
	CameraDirection cameraDirection;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraDirection |= CAMERA_FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraDirection |= CAMERA_BACK;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDirection |= CAMERA_LEFT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDirection |= CAMERA_RIGHT;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraDirection |= CAMERA_UP;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		cameraDirection |= CAMERA_DOWN;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		if (!enableMouseMove)
		{
			enableMouseMove = true;
			mousePosition = glm::vec2(floor(x), floor(y));
		}

		MouseLook(glm::vec2(floor(x), floor(y)));
	}
	else
	{
		ChangePitch(0.0f);
		ChangeYaw(0.0f);
		enableMouseMove = false;
	}

		

	if (cameraDirection != 0)
		Move(cameraDirection);
}

void CFreeFlightCamera::MouseLook(glm::vec2 currentPosition)
{
	// The whole mouse movement methodology needs improvement. It's quick/dirty and works *for now*
	// but it will struggle with small/low speed mouse movements.

	if (!enableMouseMove)
		return;

	glm::vec2 mouseDelta = mousePosition - glm::vec2(currentPosition.x, currentPosition.y);

	ChangeYaw(eulerSensitivity.x * mouseDelta.x);
	ChangePitch(eulerSensitivity.y * mouseDelta.y);

	mousePosition = glm::vec2(currentPosition.x, currentPosition.y);
}
