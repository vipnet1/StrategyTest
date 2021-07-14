#include "Statue.h"

Statue::Statue(Model* statueModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position)
	:Building(statueModel, buildingShader, BOXshader, position)
{
	this->type = BuildingType::STATUE;
	this->maxHp = 100.0f;
	this->hp = this->maxHp;

	float COLlengthX = 4.5f;
	float COLlengthZ = 4.3f;

	this->constCOLleftX = -2.25f;
	this->constCOLleftZ = -2.15f;

	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}


void Statue::Draw(bool isMovingBuildingColiding)
{
	//projection, view set up in the application file

	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model3 = glm::scale(model3, glm::vec3(0.0015f));
	this->buildingShader->SetUniformMatrix4fv("model", model3);
	this->buildingShader->SetUniform1f("hp", this->hp / this->maxHp);

	if (isMovingBuildingColiding)
		this->buildingShader->SetUniform1i("imMovingColliding", 1);
	else
		this->buildingShader->SetUniform1i("imMovingColliding", 0);

	this->buildingModel->Draw(*this->buildingShader);
}

void Statue::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::scale(model, glm::vec3(4.5f, 1.0f, 4.3f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
