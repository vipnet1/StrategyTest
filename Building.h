#pragma once

#include "Model.h"
#include "ColisionBox.h"

enum class BuildingType
{
	NONE = -1, TARGET = 0, PYRAMID = 1, STATUE = 2
};

class Building
{
protected:
	Model* buildingModel;
	glm::vec3 position;
	BuildingType type;

	float maxHp;
	float hp;

	ColisionBox* colisionBox;

	float constCOLleftX;
	float constCOLleftZ;

	const Shader* buildingShader;
	const Shader* BOXshader;

public:
	Building(Model* buildingModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position);
	virtual void Draw(bool isMovingBuildingColiding) = 0;

	void SetPosition(glm::vec3 pos);
	inline glm::vec3 GetPosition() { return this->position; }

	inline BuildingType GetType() { return this->type; }

	inline bool CheckColision_AABB(ColisionBox* secondColisionBox) { return colisionBox->CheckColision_AABB(secondColisionBox); }

	virtual void DrawBounds() = 0;

	inline ColisionBox* GetColisionBox() { return colisionBox; }

	void SetColisionBox(ColisionBox* colBox);
};

