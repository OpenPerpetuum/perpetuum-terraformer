#include "stdafx.h"
#include "ICamera.h"



ICamera::ICamera(float initialVelocity, glm::vec3 initialSensitivity, CameraType mode)
{
	cameraMode = mode;
	cameraVelocity = initialVelocity;
	eulerSensitivity = glm::vec3(initialSensitivity); // Copy it across to prevent accidental manipulation
	moveCamera = false;
	Reset();
	cameraPitch = cameraYaw = 0.f;
}


ICamera::~ICamera()
{
}

void ICamera::Reset()
{
	cameraUp = glm::vec3(0.f, 1.f, 0.f);
	fieldOfView = 45;
	position = glm::vec3(0.f, 0.f, -1.f);
	positionDelta = glm::vec3(0, 0, 0);
}

void ICamera::UpdateView()
{
	direction = glm::normalize(lookAt - position);
	glViewport(viewportX, viewportY, windowWidth, windowHeight);

	if (cameraMode == ORTHO)
		projection = glm::ortho(-1.5f * aspect, 1.5f * aspect, -1.5f, 1.5f, -10.0f, 10.f);
	else if (cameraMode == FREE)
	{
		projection = glm::perspective(fieldOfView, aspect, nearClip, farClip);
		// Quaternion math is easier to do than matrix math imo
		// Note that this method still introduces gimball lock, but will be easier (in general)
		// for people to understand (that 100% quaternion representation)
		glm::vec3 axis = glm::cross(direction, cameraUp);
		glm::quat pitchQ = glm::angleAxis(cameraPitch, axis);
		glm::quat yawQ = glm::angleAxis(cameraYaw, cameraUp);
		glm::quat rotation = glm::cross(pitchQ, yawQ);
		rotation = glm::normalize(rotation);

		direction = glm::rotate(rotation, direction);
		position += positionDelta;
		lookAt = position + direction;
		positionDelta = positionDelta * .8f; // Very basic frame-frame smoothing.
		cameraPitch = cameraYaw = 0; // Reset the Pitch/Yaw. We don't want this continuing after we stop the mouse - makes it feel "goopy"
	}

	view = glm::lookAt(position, lookAt, cameraUp);
	model = glm::mat4(1.f);
	MVP = projection * view * model;
}

void ICamera::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void ICamera::LookAt(glm::vec3 newCenter)
{
	lookAt = newCenter;
}

void ICamera::SetFOV(float fov)
{
	fieldOfView = fov;
}

void ICamera::SetViewport(int x, int y, int width, int height)
{
	viewportX = x;
	viewportY = y;
	windowWidth = width;
	windowHeight = height;

	aspect = (float)width / (float)height;
}

void ICamera::SetClipping(float nearDistance, float farDistance)
{
	nearClip = nearDistance;
	farClip = farDistance;
}

CameraType ICamera::GetMode()
{
	return cameraMode;
}

void ICamera::GetViewport(int &x, int &y, int &width, int &height)
{
	x = viewportX;
	y = viewportY;
	width = windowWidth;
	height = windowHeight;
}

void ICamera::GetMatrices(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model, glm::mat4 &mvp)
{
	projection = this->projection;
	view = this->view;
	model = this->model;
	mvp = this->MVP;
}

void ICamera::ModifySpeed(float changeInSpeed)
{
	cameraVelocity += changeInSpeed;
}