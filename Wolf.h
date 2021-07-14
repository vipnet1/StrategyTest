#pragma once

#include "Animal.h"

class Wolf : public Animal
{

public:
	Wolf(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* wolfModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position = glm::vec3(-5.0f, -0.5f, -5.0f));
	void Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime);

	void DrawBounds();
};

