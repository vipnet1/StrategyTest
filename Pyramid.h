#pragma once

#include "Building.h"

class Pyramid : public Building
{
private:

public:
	Pyramid(Model* pyramidModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position);

	void Draw(bool isMovingBuildingColiding);
	void DrawBounds();
};

