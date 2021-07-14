#include "Wolf.h"


Wolf::Wolf(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* wolfModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position)
	: Animal(spectreZoneSize, soundEngine, wolfModel, animalShader, outlineShader, BOXshader, terrain, position)
{
	this->type = AnimalType::WOLF;
	takePath();

	float COLlengthX = 0.3f;
	float COLlengthZ = 1.25f;

	this->constCOLleftX = -0.145f;
	this->constCOLleftZ = -0.625f;

	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}

void Wolf::Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime)
{
	if (tookPath || !selected)
	{
		this->position += this->front * deltaTime * speed;
		float height = this->terrain->getHeightOfTerrain(this->position.x, this->position.z);
		this->position.y = height - 0.5f;
		if (abs(this->position.x - this->currPathEndPos.x) < 0.5f && abs(this->position.z - this->currPathEndPos.z) < 0.5f)
		{
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


void Wolf::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(0.005f, 0.0f, 0.0f));
	model = glm::rotate(model, atan(this->front.x / this->front.z), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 2.0f, 1.25f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}