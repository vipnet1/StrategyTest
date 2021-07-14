#include "Renderer.h"

Renderer::Renderer(const Cave* cave, const TextRenderer* textRenderer, const ScoreHolder* scoreHolder, const BuildingHolder* buildingHolder,
	const SelectableManager* selectableManager, const InfluencePointGenerator* IFgenerator, const Terrain* terrain, const Shader* terrainShader,
	const Shader* animalShader, const Shader* IFpointShader, const Shader* buildingShader, const Shader* outline, const Shader* BOXshader, float widthFactor, float heightFactor)
	:cave(cave), textRenderer(textRenderer), scoreHolder(scoreHolder), buildingHolder(buildingHolder), selectableManager(selectableManager),
	IFgenerator(IFgenerator), terrain(terrain), terrVcount(this->terrain->getVerticesCount()), terrainShader(terrainShader), animalShader(animalShader),
	IFpointShader(IFpointShader), buildingShader(buildingShader), outline(outline), BOXshader(BOXshader), widthFactor(widthFactor), heightFactor(heightFactor)
{
}

void Renderer::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, bool isMovingBuildingColliding)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);

	glm::mat4 model(1.0f);

	this->terrain->Bind();
	this->terrainShader->SetUniformMatrix4fv("model", model);
	this->terrainShader->SetUniformMatrix4fv("projection", projectionMatrix);
	this->terrainShader->SetUniformMatrix4fv("view", viewMatrix);

	glDrawElements(GL_TRIANGLES, this->terrVcount, GL_UNSIGNED_INT, nullptr);

	int BES = (int)this->scoreHolder->GetBlueEssence();
	int BESPS = (float)this->scoreHolder->GetBlueEssenceIncome();
	std::string BSShower = std::to_string(BES) + " + " + std::to_string(BESPS);
	this->textRenderer->RenderText(BSShower, 100.0f * this->widthFactor, (MY_RESOLUTION_HEIGHT - 50.0f) * this->heightFactor, 0.75f * std::fminf(this->widthFactor, this->heightFactor), glm::vec3(0.0f, 0.0f, 1.0f));
	
	this->IFpointShader->Bind();
	this->IFpointShader->SetUniformMatrix4fv("view", viewMatrix);
	this->IFpointShader->SetUniformMatrix4fv("projection", projectionMatrix);
	this->IFgenerator->DrawInfluencePoints();

	this->buildingShader->Bind();
	this->buildingShader->SetUniformMatrix4fv("projection", projectionMatrix);
	this->buildingShader->SetUniformMatrix4fv("view", viewMatrix);
	this->buildingHolder->Draw(isMovingBuildingColliding);

	this->BOXshader->Bind();
	this->BOXshader->SetUniformMatrix4fv("projection", projectionMatrix);
	this->BOXshader->SetUniformMatrix4fv("view", viewMatrix);

	this->buildingHolder->DrawBounds();
	this->IFgenerator->DrawBounds();
	this->cave->DrawBounds();

	this->animalShader->Bind();
	this->animalShader->SetUniformMatrix4fv("projection", projectionMatrix);
	this->animalShader->SetUniformMatrix4fv("view", viewMatrix);

	this->cave->DrawCave();

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0XFF);

	this->cave->DrawAnimals();

	glDisable(GL_DEPTH_TEST);

	glStencilMask(0x00);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	this->outline->Bind();
	this->outline->SetUniformMatrix4fv("view", viewMatrix);
	this->outline->SetUniformMatrix4fv("projection", projectionMatrix);
	this->cave->DrawOutlineSelected();

	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0XFF);
}
