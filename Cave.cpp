#include "Cave.h"


Cave::Cave(float posx, float posz, unsigned int nrWolves, unsigned int nrPandas, unsigned int nrTurtles, int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* wolfModel, Model* pandaModel, Model* turtleModel, Model* caveModel, float caveHeight, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* animalShader, const Shader* outlineShader, const Terrain* terrain)
	:BOXshader(BOXshader), animalShader(animalShader), outlineShader(outlineShader), terrain(terrain)
{
	this->caveHeight = caveHeight;
	this->wolfModel = wolfModel;
	this->pandaModel = pandaModel;
	this->turtleModel = turtleModel;
	this->caveModel = caveModel;
	this->spectreZoneSize = spectreZoneSize;
	this->soundEngine = soundEngine;
	this->posx = posx;
	this->posz = posz;
	this->selected = true;

	for (int i = 0; i < nrWolves; i++)
	{
		this->animals.push_back(new Wolf(spectreZoneSize, soundEngine, wolfModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(posx, -0.5f, posz)));
	}

	for (int i = 0; i < nrPandas; i++)
	{
		this->animals.push_back(new Panda(spectreZoneSize, soundEngine, pandaModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(posx, -0.5f, posz)));
	}

	for (int i = 0; i < nrTurtles; i++)
	{
		this->animals.push_back(new Turtle(spectreZoneSize, soundEngine, turtleModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(posx, -0.5f, posz)));
	}
	this->wolfesCount = nrWolves;
	this->pandasCount = nrPandas;
	this->turtlesCount = nrTurtles;

	this->BOXva = BOXva;
	this->BOXvb = BOXvb;
}

void Cave::DrawAnimals() const
{
	for (int i = 0; i < animals.size(); i++)
	{
		this->animals[i]->Draw();
	}
}

void Cave::DrawOutlineSelected() const
{
	for (int i = 0; i < animals.size(); i++)
	{
		animals[i]->DrawOutline();
	}
}

void Cave::DrawCave() const
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(this->posx, this->caveHeight - 0.5f, this->posz));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f));
	animalShader->SetUniformMatrix4fv("model", model);

	caveModel->Draw(*animalShader);
}

void Cave::MoveAnimals(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime)
{
	for (int i = 0; i < animals.size(); i++)
	{
		this->animals[i]->Move(deltaTime, playerPos, playerHp, glfwTime);
	}
}

void Cave::takePathForSelected(glm::vec3 pos)
{
	for (int i = 0; i < this->animals.size(); i++)
	{
		animals[i]->takePath(pos);
	}
}


void Cave::UpdateSelectedStatus(float initXPos, float initZPos, float currXPos, float currZPos)
{
	for (int i = 0; i < this->animals.size(); i++)
	{
		animals[i]->CheckSelected(initXPos, initZPos, currXPos, currZPos);
	}
}

void Cave::selectAnimalType(AnimalType type)
{
	if (type == AnimalType::ALL)
	{
		for (int i = 0; i < this->animals.size(); i++)
		{
			animals[i]->SetSelected(true);
		}
	}
	else if (type == AnimalType::WOLF)
	{
		for (int i = 0; i < this->animals.size(); i++)
		{
			if(animals[i]->GetAnimalType() == AnimalType::WOLF)
				animals[i]->SetSelected(true);
		}
	}
	else if (type == AnimalType::PANDA)
	{
		for (int i = 0; i < this->animals.size(); i++)
		{
			if (animals[i]->GetAnimalType() == AnimalType::PANDA)
				animals[i]->SetSelected(true);
		}
	}
	else if (type == AnimalType::TURTLE)
	{
		for (int i = 0; i < this->animals.size(); i++)
		{
			if (animals[i]->GetAnimalType() == AnimalType::TURTLE)
				animals[i]->SetSelected(true);
		}
	}
}

int Cave::GetAnimalCount(AnimalType aType) const
{
	switch (aType)
	{
	case AnimalType::ALL:
		return this->animals.size();
	case AnimalType::WOLF:
		return this->wolfesCount;
	case AnimalType::PANDA:
		return this->pandasCount;
	case AnimalType::TURTLE:
		return this->turtlesCount;
	default:
		return -1;
	}
}

float* Cave::GetAnimalPositionsNormalizedNotSelected(float heightToWidthRatio)
{
	int nrNotSelected = this->GetAnimalCount(AnimalType::ALL) - this->GetNrSelectedAnimals();
	for (int i = 0; i < animals.size(); i++)
	{
		if (!animals[i]->GetSelected())
			nrNotSelected++;
	}

	float* positions = new float[nrNotSelected * sizeof(float) * 2];
	for (int i = 0, k = 0; i < animals.size(); i++)
	{
		if (!animals[i]->GetSelected())
		{
			glm::vec3 position = animals[i]->GetPosition();
			positions[2 * k] = 1.0f + position.x / (WORLD_SIZE * 2.0f / heightToWidthRatio);
			positions[2 * k + 1] = -1.0f - position.z / (WORLD_SIZE * 2.0f);
			k++;
		}
	}
	return positions;
}

float* Cave::GetAnimalPositionsNormalizedSelected(float heightToWidthRatio)
{
	int nrSelected = this->GetNrSelectedAnimals();

	float* positions = new float[nrSelected * sizeof(float) * 2];
	for (int i = 0, k = 0; i < animals.size(); i++)
	{
		if (animals[i]->GetSelected())
		{
			glm::vec3 position = animals[i]->GetPosition();
			positions[2 * k] = 1.0f + position.x / (WORLD_SIZE * 2.0f / heightToWidthRatio);
			positions[2 * k + 1] = -1.0f - position.z / (WORLD_SIZE * 2.0f);
			k++;
		}
	}
	return positions;
}

std::vector<glm::vec3> Cave::GetTurtlePositions()
{
	std::vector<glm::vec3> turtlePositions;
	for (int i = 0; i < this->animals.size(); i++)
	{
		if (animals[i]->GetAnimalType() == AnimalType::TURTLE)
			turtlePositions.push_back(animals[i]->GetPosition());
	}
	return turtlePositions;
}

int Cave::GetNrSelectedAnimals()
{
	int num = 0;
	for (int i = 0; i < this->animals.size(); i++)
	{
		if (animals[i]->GetSelected())
			num++;
	}
	return num;
}

void Cave::AddAnimal(AnimalType type)
{
	switch (type)
	{
	case AnimalType::WOLF:
		this->animals.push_back(new Wolf(this->spectreZoneSize, this->soundEngine, this->wolfModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(this->posx, -0.5f, this->posz)));
		this->wolfesCount++;
		break;
	case AnimalType::PANDA:
		this->animals.push_back(new Panda(this->spectreZoneSize, this->soundEngine, this->pandaModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(this->posx, -0.5f, this->posz)));
		this->pandasCount++;
		break;
	case AnimalType::TURTLE:
		this->animals.push_back(new Turtle(this->spectreZoneSize, this->soundEngine, this->turtleModel, this->animalShader, this->outlineShader, this->BOXshader, this->terrain, glm::vec3(this->posx, -0.5f, this->posz)));
		this->turtlesCount++;
		break;
	}
}


void Cave::DrawBounds() const
{
	BOXva->Bind();
	BOXvb->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < this->animals.size(); i++)
	{
		this->animals[i]->DrawBounds();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cave::CheckAnimal_AnimalColision(float deltaTime)
{
	for (int i = 0; i < this->animals.size(); i++)
	{
		for (int j = i + 1; j < this->animals.size(); j++)
		{
			if (this->animals[i]->GetColisionBox()->CheckColision_AABB_MaxRib(this->animals[j]->GetColisionBox()) && this->animals[i]->GetColisionBox()->CheckColision_SAT(this->animals[j]->GetColisionBox()) && this->animals[j]->GetColisionBox()->CheckColision_SAT(this->animals[i]->GetColisionBox()))
			{
				this->animals[i]->MoveBack(deltaTime);
				this->animals[i]->MoveBack(deltaTime);
			}
		}
	}
}

std::vector<ColisionBox*> Cave::GetAnimalColisionBoxes()
{
	std::vector<ColisionBox*> colisionBoxes;
	for (int i = 0; i < this->animals.size(); i++)
	{
		colisionBoxes.push_back(this->animals[i]->GetColisionBox());
	}
	return colisionBoxes;
}