#include "AnimalSelectable.h"

#define WOLF_COST 30
#define PANDA_COST 50
#define TURTLE_COST 80

AnimalSelectable::AnimalSelectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding, ScoreHolder* scoreHolder, float widthFactor, float heightFactor, Cave* cave)
	:Selectable(locXNormalized, locYNormalized, tr, selectableEnding), scoreHolder(scoreHolder), widthFactor(widthFactor), heightFactor(heightFactor), cave(cave)
{
	mainTexture = new Texture("res/textures/images/texBorderAnimal.jpg", 0);

	images.push_back(new Texture("res/textures/images/wolf.jpg", 0));
	images.push_back(new Texture("res/textures/images/panda.jpg", 0));
	images.push_back(new Texture("res/textures/images/turtle.jpg", 0));
	images.push_back(new Texture("res/textures/images/hammer.jpg", 0));

	this->animalCost = new float[3]; //size should be number of animals
	this->animalCost[0] = WOLF_COST;
	this->animalCost[1] = PANDA_COST;
	this->animalCost[2] = PANDA_COST;
}

ImgAnimal AnimalSelectable::FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized)
{
	for (int i = 0; i < this->images.size(); i++)
	{
		if (cursorXPosNormalized < -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * (i + 1))) && cursorXPosNormalized > -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * i)) && cursorYPosNormalized < selectableEnding.y)
		{
			if ((ImgAnimal)i == ImgAnimal::IMG_HAMMER)
				return ImgAnimal::IMG_HAMMER;

			if (scoreHolder->TryToBuy(this->animalCost[i]))
				return (ImgAnimal)i;
			else
				return ImgAnimal::NONE;
		}

	}
	return ImgAnimal::NONE;
}

void AnimalSelectable::Draw()
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

		this->tr->RenderText(std::to_string(cave->GetAnimalCount((AnimalType)i)), currTextXAmountAnimals * widthFactor, 80.0f * heightFactor, 0.5f * fmin(widthFactor, heightFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		currTextXAmountAnimals += 120.0f;


		int cost = this->animalCost[i];
		if (cost < 100)
			this->tr->RenderText(std::to_string(cost), currTextXCostAnimals * widthFactor, 10.0f * heightFactor, 0.5f * fmin(widthFactor, heightFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		else
			this->tr->RenderText(std::to_string(cost), (currTextXCostAnimals - 12.0f) * widthFactor, 10.0f * heightFactor, 0.5f * fmin(widthFactor, heightFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		currTextXCostAnimals += 120.0f;
	}
}
