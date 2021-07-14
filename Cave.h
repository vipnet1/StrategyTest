#pragma once
#include<vector>

#include "Wolf.h"
#include "Panda.h"
#include "Turtle.h"

class Cave
{
private:
	std::vector<Animal*> animals;
	float posx, posz;
	bool selected;
	int spectreZoneSize;
	Model* wolfModel, *pandaModel, *turtleModel;
	Model* caveModel;
	irrklang::ISoundEngine* soundEngine;

	int wolfesCount;
	int pandasCount;
	int turtlesCount;

	float caveHeight;

	VertexArray* BOXva;
	VertexBuffer* BOXvb;

	const Shader* BOXshader;
	const Shader* animalShader;
	const Shader* outlineShader;

	const Terrain* terrain;

public:
	Cave(float posx, float posz, unsigned int nrWolves, unsigned int nrPandas, unsigned int nrTurtles, int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* wolfModel, Model* pandaModel, Model* turtleModel, Model* caveModel, float caveHeight, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* animalShader, const Shader* outlineShader, const Terrain* terrain);
	void DrawAnimals() const;
	void DrawOutlineSelected() const;
	void DrawCave() const;

	void MoveAnimals(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime);
	void takePathForSelected(glm::vec3 pos);
	void UpdateSelectedStatus(float initXPos, float initZPos, float currXPos, float currZPos);
	inline bool GetSelected() { return selected; }
	void selectAnimalType(AnimalType type);

	int GetAnimalCount(AnimalType aType) const;
	float* GetAnimalPositionsNormalizedNotSelected(float heightToWidthRatio);
	float* GetAnimalPositionsNormalizedSelected(float heightToWidthRatio);

	std::vector<glm::vec3> GetTurtlePositions();
	int GetNrSelectedAnimals();

	void AddAnimal(AnimalType type);
	void DrawBounds() const;

	void CheckAnimal_AnimalColision(float deltaTime);

	std::vector<ColisionBox*> GetAnimalColisionBoxes();
};

