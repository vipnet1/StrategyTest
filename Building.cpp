#include "Building.h"

Building::Building(Model* buildingModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position)
	:BOXshader(BOXshader), buildingShader(buildingShader)
{
	this->buildingModel = buildingModel;
	this->position = position;
}

void Building::SetPosition(glm::vec3 pos)
{
	this->position = pos;
	this->colisionBox->topLeftX = this->constCOLleftX + pos.x;
	this->colisionBox->topLeftZ = this->constCOLleftZ + pos.z;
}

void Building::SetColisionBox(ColisionBox* colBox)
{
	this->colisionBox = colBox;
}
