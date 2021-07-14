#pragma once

#include "Camera.h"
#include "Terrain.h"


class TerrainCamera : public Camera
{
private:
	const Terrain* terrain;
	const float gravitation;

	float ySpeed;
	bool isFalling;

	float playerHp;

public:
	TerrainCamera(const Terrain* terrain, float gravitation = 9.8f, float cameraSpeed = 2.5f, glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
	void Move(Direction dir, float deltaTime);
	void ApplyGravitation(float deltaTime);
	void TryToJump();
	inline float* GetPlayerHp() { return &playerHp; }
};