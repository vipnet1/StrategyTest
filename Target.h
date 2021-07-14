#pragma once

#include "Building.h"

class Target : public Building
{
private:

public:
	Target(Model* targetModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position);

	void Draw(bool isMovingBuildingColiding);
	void DrawBounds();
};

