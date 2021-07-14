#include "InfluencePoint.h"

#define CAPTURE_RADIUS 2.0f

InfluencePoint::InfluencePoint(float posx, float posz, Model* treeModel, Model* holyTreeModel, const Terrain* terrain, const ScoreHolder* scoreHolder, const Shader* IFshader, const Shader* BOXshader, const Shader *BARshader)
	:OnCaptureValue(100), CapturePerSecond(5.0f), DecCapturePerSecond(10.0f), scoreHolder(scoreHolder), IFshader(IFshader), BOXshader(BOXshader), BARshader(BARshader)
{
	this->captured = false;
	this->currCaptureVal = 0.0f;
	this->position = glm::vec3(posx, terrain->getHeightOfTerrain(posx, posz) - 0.5f, posz);
	this->holyTreeModel = holyTreeModel;
	this->treeModel = treeModel;

	this->colisionBox = new ColisionBox(-1.2f + posx, -1.2f + posz, 2.4f, 2.4f);
}

void InfluencePoint::Draw()
{
	glm::mat4 model3(1.0f);
	model3 = glm::translate(model3, this->position);
	model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	model3 = glm::scale(model3, glm::vec3(0.015f));
	this->IFshader->SetUniformMatrix4fv("model", model3);
	if(this->currCaptureVal == 100.0f)
		this->holyTreeModel->Draw(*this->IFshader);
	else
		this->treeModel->Draw(*this->IFshader);
}

void InfluencePoint::DrawBar(glm::vec3 pos)
{

	if (this->currCaptureVal == 100.0f)
	{
		this->BARshader->SetUniform1i("bold", 1);
	}
	else
		this->BARshader->SetUniform1i("bold", 0);

	if (true)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(0.25f, 0.03f, 1.0f));
		this->BARshader->SetUniformMatrix4fv("model", model);
		this->BARshader->SetUniform1i("blue", 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}

	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(0.25f, 0.03f, 1.0f));
		model = glm::scale(model, glm::vec3(this->currCaptureVal / 100.0f, 1.0f, 1.0f));
		this->BARshader->SetUniformMatrix4fv("model", model);
		this->BARshader->SetUniform1i("blue", 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}
}

void InfluencePoint::Capture(std::vector<glm::vec3> turtlePosition, float deltaTime)
{
	if (this->currCaptureVal != 100.0f)
	{
		for (int i = 0; i < turtlePosition.size(); i++)
		{
			if (glm::length(turtlePosition[i] - this->position) < CAPTURE_RADIUS)
			{
				this->currCaptureVal += deltaTime * CapturePerSecond;
				if (this->currCaptureVal > 100.0f)
				{
					this->currCaptureVal = 100.0f;
					this->scoreHolder->incNrIfPointsTaken();
				}
				return;
			}
		}
		if (this->currCaptureVal != 0.0f)
		{
			this->currCaptureVal -= deltaTime * DecCapturePerSecond;
			if (this->currCaptureVal < 0.0f)
				this->currCaptureVal = 0.0f;
		}
	}
}


void InfluencePoint::DrawBounds()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->position);
	model = glm::scale(model, glm::vec3(2.4f, 1.0f, 2.4f));
	this->BOXshader->SetUniformMatrix4fv("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}