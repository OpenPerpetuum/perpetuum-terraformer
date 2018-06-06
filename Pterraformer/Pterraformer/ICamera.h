#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <bitset>

enum CameraType {
	ORTHO, FREE
};

#define CAMERA_UP 1 << 0
#define CAMERA_DOWN 1 << 1
#define CAMERA_LEFT 1 << 2
#define CAMERA_RIGHT 1 << 3
#define CAMERA_FORWARD 1 << 4
#define CAMERA_BACK 1 << 5

typedef std::bitset<6> CameraDirection;

class ICamera
{
public:
	ICamera(float initalVelocity, glm::vec3 initialSensitivity, CameraType type);
	~ICamera();

	virtual void Move(CameraDirection direction) = 0;
	virtual void ChangePitch(float degrees) = 0;
	virtual void ChangeYaw(float degress) = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;

	virtual void Reset();
	virtual void UpdateView();
	virtual void SetPosition(glm::vec3 newPosition);
	virtual void LookAt(glm::vec3 newCenter);
	virtual void SetFOV(float fov);
	virtual void SetViewport(int x, int y, int width, int height);
	virtual void SetClipping(float nearClipDistance, float farClipDistance);

	CameraType GetMode();
	void GetViewport(int &x, int &y, int &width, int &height);
	void GetMatrices(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);
	void ModifySpeed(float changeInSpeed);
protected:
	CameraType cameraMode;

	int viewportX, viewportY;
	int windowWidth, windowHeight;

	float aspect;
	float fieldOfView;
	float nearClip;
	float farClip;

	float cameraYaw, cameraPitch;
	glm::vec3 eulerSensitivity; // If we switch to full quat representation we can remove the previous 3 vars and the 5 vec3 declarations below
	float cameraVelocity;

	bool moveCamera;

	glm::vec3 position, positionDelta;
	glm::vec3 lookAt, direction, cameraUp;

	glm::mat4 projection, view, model, MVP;

};

