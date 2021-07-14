#include "BuildingSelectable.h"

#define TARGET_COST 100.0f
#define PYRAMID_COST 200.0f
#define STATUE_COST 50.0f

BuildingSelectable::BuildingSelectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding, ScoreHolder* scoreHolder, float widthFactor, float heightFactor, const BuildingHolder* buildingHolder)
	:Selectable(locXNormalized, locYNormalized, tr, selectableEnding), scoreHolder(scoreHolder), widthFactor(widthFactor), heightFactor(heightFactor), buildingHolder(buildingHolder)
{
	this->mainTexture = new Texture("res/textures/images/texBorderBuilding.jpg", 0);

	images.push_back(new Texture("res/textures/images/target.jpg", 0));
	images.push_back(new Texture("res/textures/images/pyramid.jpg", 0));
	images.push_back(new Texture("res/textures/images/statue.jpg", 0));
	images.push_back(new Texture("res/textures/images/parrot.jpg", 0));

	this->buildingCost = new float[3]; //size should be current building number
	this->buildingCost[0] = TARGET_COST;
	this->buildingCost[1] = PYRAMID_COST;
	this->buildingCost[2] = STATUE_COST;
}

ImgBuilding BuildingSelectable::FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized)
{
	for (int i = 0; i < this->images.size(); i++)
	{
		if (cursorXPosNormalized < -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * (i + 1))) && cursorXPosNormalized > -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * i)) && cursorYPosNormalized < selectableEnding.y)
		{

			if ((ImgBuilding)i == ImgBuilding::IMG_PARROT)
				return ImgBuilding::IMG_PARROT;

			if (this->buildingHolder->IsBuildingMovingNow())
				return (ImgBuilding)i;
			else if(scoreHolder->TryToBuy(this->buildingCost[i]))
				return (ImgBuilding)i;
			else
				return ImgBuilding::NONE;
		}

	}
	return ImgBuilding::NONE;
}

void BuildingSelectable::Draw()
{
	va->Bind();
	shader->Bind();
	ib->Bind();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(this->locXNormalized, this->locYNormalized, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
	shader->SetUniformMatrix4fv("model", model);
	mainTexture->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.9375f, this->locYNormalized, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 0.2f, 1.0f));
	for (int i = 0; i < images.size(); i++)
	{
		images[i]->Bind();
		shader->SetUniformMatrix4fv("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
		model = glm::translate(model, glm::vec3(1.01f, 0.0f, 0.0f));
	}

	int currTextXAmountAnimals = 10.0f;
	int currTextXCostAnimals = 90.0f;
	for (int i = 0; i < images.size() - 1; i++)
	{
		int cost = this->buildingCost[i];
		if (cost < 100)
			this->tr->RenderText(std::to_string(cost), currTextXCostAnimals * widthFactor, 10.0f * heightFactor, 0.5f * fmin(widthFactor, heightFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		else
			this->tr->RenderText(std::to_string(cost), (currTextXCostAnimals - 12.0f) * widthFactor, 10.0f * heightFactor, 0.5f * fmin(widthFactor, heightFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		currTextXCostAnimals += 120.0f;
	}
}

void BuildingSelectable::RefundCostOf(BuildingType type)
{
	this->scoreHolder->setBlueEssence(this->scoreHolder->GetBlueEssence() + buildingCost[(int)type]);
}
