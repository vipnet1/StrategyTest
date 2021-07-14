#include "Camera.h"
#include<iostream>
#include "Terrain.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, float cameraSpeed)
	:position(position), front(front), cameraSpeed(cameraSpeed), initialPos(position), initFov(45.0f)
{
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->fov = 45.0f;
}

void Camera::Move(Direction dir, float deltaTime)
{
	switch (dir)
	{
	case Direction::FORWARD:
		this->position.z -= deltaTime * this->cameraSpeed;
		break;
	case Direction::BACKWARD:
		this->position.z += deltaTime * this->cameraSpeed;
		break;
	case Direction::RIGHT:
		this->position.x += deltaTime * this->cameraSpeed;
		break;
	case Direction::LEFT:
		this->position.x -= deltaTime * this->cameraSpeed;
		break;
	}
}

void Camera::Scroll(Direction dir, float deltaTime)
{

	switch (dir)
	{
	case Direction::FORWARD:
		this->fov -= deltaTime * this->scrollPerSecond;
		break;
	case Direction::BACKWARD:
		this->fov += deltaTime * this->scrollPerSecond;
		break;
	}
	if (fov < 5.0f)
		fov = 5.0f;
	else if (fov > 85.0f)
		fov = 85.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view(1.0f);
	view = glm::lookAt(this->position, this->position + this->front, up);

	return view;
}

void Camera::SetPosition(glm::vec3 pos)
{
	this->position.x = pos.x;
	this->position.z = pos.z;
}

float* Camera::GetPositionNormalized(float heightToWidthRatio)  //for map point calculation
{
	float* pos = new float[2];

	pos[0] = 1.0f + this->position.x / (WORLD_SIZE * 2.0f / heightToWidthRatio);
	pos[1] = -1.0f - (this->position.z) / (WORLD_SIZE * 2.0f);

	return pos;
}
