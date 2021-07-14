#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "Terrain.h"
#include "irrKlang.h"
#include "ColisionBox.h"

enum class AnimalType
{
	ALL = -1, WOLF = 0, PANDA = 1, TURTLE = 2
};

class Animal
{
protected:
	glm::vec3 position;
	irrklang::ISoundEngine* soundEngine;
	glm::vec3 front;
	glm::vec3 currPathEndPos;
	float speed;
	int spectreZoneSize;
	bool selected;
	bool tookPath;
	Model* animalModel;

	bool checkPlayerNear(glm::vec3 playerPos);
	bool spawned;
	float lastAttackTime;
	AnimalType type;

	ColisionBox* colisionBox;

	float constCOLleftX;
	float constCOLleftZ;

	const Shader* animalShader;
	const Shader* outlineShader;
	const Shader* BOXshader;

	const Terrain* terrain;

public:
	Animal(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* animalModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position = glm::vec3(-5.0f, -0.5f, -5.0f));
	virtual void takePath();
	virtual void takePath(glm::vec3 position); //only if selected
	virtual void Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime) = 0;
	void MoveBack(float deltaTime);
	virtual void Draw();
	virtual void DrawOutline();
	inline bool GetChosen() { return selected; }
	void CheckSelected(float initXPos, float initZPos, float currXPos, float currZPos);
	void SetSelected(bool b);

	inline AnimalType GetAnimalType() { return type; }
	inline bool GetSelected() { return this->selected; }
	inline glm::vec3 GetPosition() { return this->position; }

	virtual void DrawBounds() = 0;
	void SetColisionBox(ColisionBox* colBox);

	inline ColisionBox* GetColisionBox() { return colisionBox; }
};

