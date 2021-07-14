#include "Target.h"

Target::Target(Model* targetModel, const Shader* buildingShader, const Shader* BOXshader, glm::vec3 position)
	:Building(targetModel, buildingShader, BOXshader, position)
{
	this->type = BuildingType::TARGET;
	this->maxHp = 100.0f;
	this->hp = this->maxHp;

	float COLlengthX = 2.4f;
	float COLlengthZ = 1.0f;

	this->constCOLleftX = -1.02f;
	this->constCOLleftZ = 0.0f;

	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}


void Target::Draw(bool isMovingBuildingColiding)
{
	//projection, view set up in the application file

	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-100.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model3 = glm::scale(model3, glm::vec3(0.003f));
	this->buildingShader->SetUniformMatrix4fv("model", model3);
	this->buildingShader->SetUniform1f("hp", this->hp / this->maxHp);

	if (isMovingBuildingColiding)
		this->buildingShader->SetUniform1i("imMovingColliding", 1);
	else
		this->buildingShader->SetUniform1i("imMovingColliding", 0);

	this->buildingModel->Draw(*this->buildingShader);
}

void Target::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(0.18f, 3.0f, 0.5f));
	model = glm::scale(model, glm::vec3(2.4f, 1.0f, 1.0f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
