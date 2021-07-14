#pragma once

#include "Building.h"

class Statue : public Building
{
private:

public:
	Statue(Model* statueModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position);

	void Draw(bool isMovingBuildingColiding);
	void DrawBounds();
};

