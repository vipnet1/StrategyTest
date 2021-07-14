#include "TerrainCamera.h"

TerrainCamera::TerrainCamera(const Terrain* terrain, float cameraSpeed, float gravitation, glm::vec3 position, glm::vec3 front)
	:Camera(position, front, cameraSpeed), gravitation(gravitation), terrain(terrain)
{
	ySpeed = 0.0f;
	playerHp = 100.0f;
}

void TerrainCamera::Move(Direction dir, float deltaTime)
{
	float oldY = position.y;
	Camera::Move(dir, deltaTime);
	position.y = oldY;
}

void TerrainCamera::ApplyGravitation(float deltaTime)
{
	ySpeed = ySpeed - gravitation * deltaTime;
	position.y += deltaTime * ySpeed;

	float terrainHeight = terrain->getHeightOfTerrain(position.x, position.z);
	if (position.y < terrainHeight)
	{
		position.y = terrainHeight;
		isFalling = false;
	}
	else
		isFalling = true;
}

void TerrainCamera::TryToJump()
{
	if (!isFalling)
		ySpeed += 5.0f;
}
