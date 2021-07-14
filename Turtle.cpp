#include "Turtle.h"

Turtle::Turtle(int spectreZoneSize, irrklang::ISoundEngine* soundEngine, Model* turtleModel, const Shader* animalShader, const Shader* outlineShader, const Shader* BOXshader, const Terrain* terrain, glm::vec3 position)
	: Animal(spectreZoneSize, soundEngine, turtleModel, animalShader, outlineShader, BOXshader, terrain, position)
{
	this->type = AnimalType::TURTLE;
	this->speed = 5.0f;
	this->TakeTurtleInitPath(this->position - 3.0f);

	float COLlengthX = 0.6f;
	float COLlengthZ = 0.85f;

	this->constCOLleftX = -0.295f;
	this->constCOLleftZ = -0.425f;

	this->SetColisionBox(new ColisionBox(this->constCOLleftX, this->constCOLleftZ, COLlengthX, COLlengthZ));
}

void Turtle::Move(float deltaTime, glm::vec3 playerPos, float* playerHp, float glfwTime)
{
	if (tookPath || !selected)
	{
		if (tookPath)
		{
			this->position += this->front * deltaTime * speed;
			float height = this->terrain->getHeightOfTerrain(this->position.x, this->position.z);
			this->position.y = height - 0.5f;
		}
		if (abs(this->position.x - this->currPathEndPos.x) < 0.5f && abs(this->position.z - this->currPathEndPos.z) < 0.5f)
		{
			tookPath = false;
			//takePath();
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

void Turtle::Draw()
{
	//projection, view set up in the application file

	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model3 = glm::rotate(model3, atan(this->front.x / this->front.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (this->front.z < 0.0f)
		model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	model3 = glm::scale(model3, glm::vec3(0.0012f));
	this->animalShader->SetUniformMatrix4fv("model", model3);
	this->animalModel->Draw(*this->animalShader);
}

void Turtle::DrawOutline()
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
		model3 = glm::scale(model3, glm::vec3(0.0012f));
		this->outlineShader->SetUniformMatrix4fv("model", model3);
		this->animalModel->Draw(*this->outlineShader);
	}
}

void Turtle::TakeTurtleInitPath(glm::vec3 position)
{
	this->front = glm::normalize(glm::vec3(position.x, 0.0f, position.z) - glm::vec3(this->position.x, 0.0f, this->position.z));
	currPathEndPos = glm::vec3(position.x, this->position.z, position.z);
	this->tookPath = true;
}


void Turtle::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::translate(model, glm::vec3(0.005f, 0.0f, 0.0f));
	model = glm::rotate(model, atan(this->front.x / this->front.z), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 2.0f, 0.85f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}