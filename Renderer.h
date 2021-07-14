#pragma once
#include "Cave.h"
#include "TextRenderer.h"
#include "ScoreHolder.h"
#include "BuildingHolder.h"
#include "SelectableManager.h"
#include "InfluencePointGenerator.h"
#include "Terrain.h"
#include "Map.h"

class Renderer
{
private:
	const Cave* cave;
	const TextRenderer* textRenderer;
	const ScoreHolder* scoreHolder;
	const BuildingHolder* buildingHolder;
	const SelectableManager* selectableManager;
	const InfluencePointGenerator* IFgenerator;
	const Terrain* terrain;
	const int terrVcount;

	const Shader* terrainShader;
	const Shader *animalShader;
	const Shader *IFpointShader;
	const Shader *buildingShader;
	const Shader *outline;
	const Shader *BOXshader;

	const float widthFactor, heightFactor;

public:
	Renderer(const Cave* cave, const TextRenderer* textRenderer, const ScoreHolder* scoreHolder, const BuildingHolder* buildingHolder,
		const SelectableManager* selectableManager, const InfluencePointGenerator* IFgenerator, const Terrain* terrain, const Shader* terrainShader, 
		const Shader* animalShader, const Shader* IFpointShader, const Shader* buildingShader, const Shader* outline, const Shader* BOXshader, float widthFactor, float heightFactor);

	void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, bool isMovingBuildingColliding);
};

