#include "Pyramid.h"

Pyramid::Pyramid(Model* pyramidModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position)
	:Building(pyramidModel, buildingShader, BOXshader, position)
{
	this->type = BuildingType::PYRAMID;
	this->maxHp = 100.0f;
	this->hp = this->maxHp;

	float COLlengthX = 4.5f;
	float COLlengthZ = 4.5f;

	this->constCOLleftX = -2.25f;
	this->constCOLleftZ = -2.25f;
	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}


void Pyramid::Draw(bool isMovingBuildingColiding)
{
	//projection, view set up in the application file

	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model3 = glm::scale(model3, glm::vec3(0.015f));
	this->buildingShader->SetUniformMatrix4fv("model", model3);
	this->buildingShader->SetUniform1f("hp", this->hp / this->maxHp);

	if(isMovingBuildingColiding)
		this->buildingShader->SetUniform1i("imMovingColliding", 1);
	else
		this->buildingShader->SetUniform1i("imMovingColliding", 0);

	this->buildingModel->Draw(*this->buildingShader);
}

void Pyramid::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.3f, 4.5f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
