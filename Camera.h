#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class Direction
{
	FORWARD = 0, BACKWARD = 1, RIGHT = 2, LEFT = 3
};

class Camera
{
protected:
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float cameraSpeed;

	const float scrollPerSecond = 30.0f;

	glm::vec3 initialPos;
	glm::vec3 position;

public:
	float fov;
	const float initFov;

	Camera(glm::vec3 position = glm::vec3(-22.5f, 30.0f, -12.6f), glm::vec3 front = glm::vec3(0.0f, -1.0f, -0.0001f), float cameraSpeed = 10.0f);

	virtual void Move(Direction dir, float deltaTime);
	void Scroll(Direction dir, float deltaTime);
	glm::mat4 GetViewMatrix();

	void SetPosition(glm::vec3 pos);

	inline glm::vec3 GetPosition() { return position; }
	inline glm::vec3 GetInitialPosition() { return initialPos; }
	float* GetPositionNormalized(float heightToWidthRatio);
};