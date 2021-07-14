#include "Animal.h"

#define RADIUS_SEEN 10.0f
#define WOLF_DMG_PER_SECOND 5.0f;

bool Animal::checkPlayerNear(glm::vec3 playerPos)
{
	float dis = glm::length(playerPos - glm::vec3(this->position.x, 0.0f, this->position.z));
	if (dis < RADIUS_SEEN)
	{
		currPathEndPos = glm::vec3(this->position.x, 0.0f, this->position.z);
		this->front = glm::normalize(glm::vec3(playerPos.x, 0.0f, playerPos.z) - glm::vec3(this->position.x, 0.0f, this->position.z));
		return true;
	}
	return false;
}

Animal::Animal(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* animalModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position)
	:animalShader(animalShader), outlineShader(outlineShader), BOXshader(BOXshader), terrain(terrain)
{
	this->selected = false;
	this->soundEngine = soundEngine;
	this->spectreZoneSize = spectreZoneSize;
	this->position = position;
	this->spawned = false;
	this->lastAttackTime = 0.0f;
	this->animalModel = animalModel;
	this->speed = 2.5f;
}

void Animal::takePath()
{
	if (!selected)
	{
		int posx = -(rand() % spectreZoneSize);
		int posz = -(rand() % spectreZoneSize);
		this->front = glm::normalize(glm::vec3(posx, 0.0f, posz) - glm::vec3(this->position.x, 0.0f, this->position.z));
		currPathEndPos = glm::vec3(posx, this->position.z, posz);
		this->tookPath = true;
	}
}

void Animal::takePath(glm::vec3 position)
{
	if (this->selected)
	{
		this->front = glm::normalize(glm::vec3(position.x, 0.0f, position.z) - glm::vec3(this->position.x, 0.0f, this->position.z));
		currPathEndPos = glm::vec3(position.x, this->position.z, position.z);
		this->tookPath = true;
	}
}

void Animal::MoveBack(float deltaTime)
{
	this->position -= this->front * deltaTime * speed;
	float height = this->terrain->getHeightOfTerrain(this->position.x, this->position.z);
	this->position.y = height - 0.5f;
}

void Animal::Draw()
{
	//projection, view set up in the application file

	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model3 = glm::rotate(model3, atan(this->front.x / this->front.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (this->front.z < 0.0f)
		model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	model3 = glm::scale(model3, glm::vec3(0.0006f));
	this->animalShader->SetUniformMatrix4fv("model", model3);
	this->animalModel->Draw(*this->animalShader);
}

void Animal::DrawOutline()
{
	if (this->selected)
	{
		glm::mat4 model3(1.0f);
		model3 = glm::translate(model3, this->position);
		model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model3 = glm::rotate(model3, atan(this->front.x / this->front.z), glm::vec3(0.0f, 0.0f, 1.0f));
		if (this->front.z < 0.0f)
			model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		model3 = glm::scale(model3, glm::vec3(1.1f));
		model3 = glm::scale(model3, glm::vec3(0.0006f));
		this->outlineShader->SetUniformMatrix4fv("model", model3);
		this->animalModel->Draw(*this->outlineShader);
	}
}

void Animal::CheckSelected(float initXPos, float initZPos, float currXPos, float currZPos)
{
	if ((this->position.x > initXPos&& this->position.x < currXPos && this->position.z > initZPos&& this->position.z < currZPos)
		|| (this->position.x > currXPos&& this->position.x < initXPos && this->position.z > currZPos&& this->position.z < initZPos)
		|| (this->position.x > initXPos&& this->position.x < currXPos && this->position.z > currZPos&& this->position.z < initZPos)
		|| (this->position.x > currXPos&& this->position.x < initXPos && this->position.z > initZPos&& this->position.z < currZPos))
		this->selected = true;
	else
		this->selected = false;
}

void Animal::SetSelected(bool b)
{
	this->selected = b;
}


void Animal::SetColisionBox(ColisionBox* colBox)
{
	this->colisionBox = colBox;
}