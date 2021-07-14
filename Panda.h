#pragma once
#include "Animal.h"

class Panda : public Animal
{
private:
	glm::vec3 patrulPoint;
	bool tookPathByHuman;
	int spectreZoneSize;

public:
	Panda(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* pandaModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position = glm::vec3(-5.0f, -0.5f, -5.0f));

	void Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime);

	void takePath();
	void takePath(glm::vec3 position);

	void DrawBounds();
};

