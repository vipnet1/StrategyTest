#include "Panda.h"

#define RADIUS_PATRUL 10.0f

Panda::Panda(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* pandaModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position)
	: Animal(spectreZoneSize, soundEngine, pandaModel, animalShader, outlineShader, BOXshader, terrain, position)
{
	this->type = AnimalType::PANDA;
	this->patrulPoint = glm::vec3(position.x, 0.0f, position.z);
	this->speed = 2.0f;
	this->tookPathByHuman = false;
	this->spectreZoneSize = spectreZoneSize;
	takePath();

	float COLlengthX = 0.7f;
	float COLlengthZ = 1.25f;

	this->constCOLleftX = -0.4f;
	this->constCOLleftZ = -0.625f;

	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}

void Panda::Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime)
{
	if (tookPath || !selected)
	{
		this->position += this->front * deltaTime * speed;
		float height = this->terrain->getHeightOfTerrain(this->position.x, this->position.z);
		this->position.y = height - 0.5f;

		bool tkPath = false;

		if (this->position.x > 0.0f)
		{
			this->position.x = 0.0f;
			tkPath = true;
		}
		else if (this->position.x < -spectreZoneSize)
		{
			this->position.x = -spectreZoneSize;
			tkPath = true;
		}

		if (this->position.z > 0.0f)
		{
			this->position.z = 0.0f;
			tkPath = true;
		}
		else if (this->position.z < -spectreZoneSize)
		{
			this->position.z = -spectreZoneSize;
			tkPath = true;
		}

		if ((abs(this->position.x - this->currPathEndPos.x) < 0.5f && abs(this->position.z - this->currPathEndPos.z) < 0.5f)
			|| (length(glm::vec2(this->position.x, this->position.z) - glm::vec2(this->patrulPoint.x, this->patrulPoint.z)) > RADIUS_PATRUL && !this->tookPathByHuman)
			|| tkPath)
		{
			this->tookPathByHuman = false;
			tookPath = false;
			takePath();
		}
		/*
		if (checkPlayerNear(playerPos))
		{
			if (!this->spawned)
				soundEngine->play2D("res/sounds/howl.mp3");
			this->spawned = true;
			this->position += this->front * deltaTime * speed;
			if (abs(this->position.x - playerPos.x) < 0.5f && abs(this->position.z - playerPos.z) < 0.5f && abs((this->position.y + 0.5f) - playerPos.y) < 0.5f)
			{
				if (glfwTime - this->lastAttackTime > 1.0f)
				{
					this->lastAttackTime = glfwTime;
					soundEngine->play2D("res/sounds/bite.mp3");
				}
				*playerHp -= deltaTime * WOLF_DMG_PER_SECOND;
			}
		}
		else*/
		spawned = false;
		this->colisionBox->topLeftX = this->constCOLleftX + this->position.x;
		this->colisionBox->topLeftZ = this->constCOLleftZ + this->position.z;
		this->colisionBox->angle = atan(this->front.x / this->front.z);
	}
}

void Panda::takePath()
{
	if (!selected)
	{
		float dirx = rand() % 200;
		float dirz = rand() % 200;
		dirx = dirx * 2 - 100.0f;
		dirz = dirz * 2 - 100.0f;
		float pnt = (rand() % 100) / 100.0f;
		float point = pnt * RADIUS_PATRUL;
		currPathEndPos = glm::normalize(glm::vec3(dirx, 0.0f, dirz)) * point + patrulPoint;
		this->front = glm::normalize(currPathEndPos - glm::vec3(this->position.x, 0.0f, this->position.z));
		this->tookPath = true;
	}
}

void Panda::takePath(glm::vec3 position)
{
	if (this->selected)
	{
		this->front = glm::normalize(glm::vec3(position.x, 0.0f, position.z) - glm::vec3(this->position.x, 0.0f, this->position.z));
		currPathEndPos = glm::vec3(position.x, 0.0f, position.z);
		this->tookPath = true;
		this->patrulPoint = currPathEndPos;
		this->tookPathByHuman = true;
	}
}

void Panda::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(-0.05f, 0.0f, 0.0f));
	model = glm::rotate(model, atan(this->front.x / this->front.z), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 2.0f, 1.25f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}