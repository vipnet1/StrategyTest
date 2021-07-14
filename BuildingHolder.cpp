#include "BuildingHolder.h"

#include "Terrain.h"


BuildingHolder::BuildingHolder(Model* targetModel, Model* pyramidModel, Model* statueModel, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* buildingShader)
	:BOXshader(BOXshader), buildingShader(buildingShader)
{
	this->BOXva = BOXva;
	this->BOXvb = BOXvb;
	this->targetModel = targetModel;
	this->pyramidModel = pyramidModel;
	this->statueModel = statueModel;
	this->buildingMovingNow = false;
	this->movingBuilding = nullptr;
}

void BuildingHolder::Draw(bool isMovingBuildingColiding) const
{
	for (int i = 0; i < buildings.size(); i++)
	{
		buildings[i]->Draw(false);
	}
	if (this->movingBuilding)
		this->movingBuilding->Draw(isMovingBuildingColiding);
}

void BuildingHolder::AddMovingBuilding(BuildingType type)
{
	if (!this->buildingMovingNow)
	{
		switch (type)
		{
		case BuildingType::TARGET:
			this->movingBuilding = new Target(this->targetModel, this->buildingShader, this->BOXshader, glm::vec3(-10.0f, 10.0f, -10.0f));
			break;
		case BuildingType::PYRAMID:
			this->movingBuilding = new Pyramid(this->pyramidModel, this->buildingShader, this->BOXshader, glm::vec3(-10.0f, 10.0f, -10.0f));
			break;
		case BuildingType::STATUE:
			this->movingBuilding = new Statue(this->statueModel, this->buildingShader, this->BOXshader, glm::vec3(-10.0f, 10.0f, -10.0f));
			break;
		}
		this->buildingMovingNow = true;
	}
}

void BuildingHolder::MoveBuilding(float currXPos, float currZPos, float currHeight)
{
	this->movingBuilding->SetPosition(glm::vec3(currXPos, currHeight - 0.5f, currZPos));
}

void BuildingHolder::TryToPlaceMovingBuilding()
{
	if (this->buildingMovingNow)
	{
		float COLlengthX;
		float COLlengthZ;
		float COLleftX;
		float COLleftZ;

		BuildingType movingBuildingType = this->movingBuilding->GetType();

		glm::vec3 movingBuildingPos = this->movingBuilding->GetPosition();

		if (movingBuildingType == BuildingType::STATUE)
		{
			COLlengthX = 2.5f;
			COLlengthZ = 2.5f;

			COLleftX = -1.2f + movingBuildingPos.x;
			COLleftZ = -1.25f + movingBuildingPos.z;
		}
		else if (movingBuildingType == BuildingType::PYRAMID)
		{
			COLlengthX = 2.5f;
			COLlengthZ = 2.5f;

			COLleftX = -1.2f + movingBuildingPos.x;
			COLleftZ = -1.25f + movingBuildingPos.z;
		}

		this->buildings.push_back(this->movingBuilding);
		this->movingBuilding = nullptr;
		this->buildingMovingNow = false;
	}
}

bool BuildingHolder::TryToCancelMovingBuilding()
{
	if (this->buildingMovingNow)
	{
		this->buildingMovingNow = false;
		this->movingBuilding = nullptr;
		return true;
	}
	return false;
}

BuildingType BuildingHolder::GetLastMovingBuildingType()
{
	if (this->movingBuilding != nullptr)
		return this->movingBuilding->GetType();
	else
		return BuildingType::NONE;
}

float* BuildingHolder::GetBuildingPositionsNormalized(float heightToWidthRatio)
{
	int nrBuildings = this->buildings.size();

	float* positions = new float[nrBuildings * sizeof(float) * 2];
	for (int i = 0; i < nrBuildings; i++)
	{
		glm::vec3 position = this->buildings[i]->GetPosition();
		positions[2 * i] = 1.0f + position.x / (WORLD_SIZE * 2.0f / heightToWidthRatio);
		positions[2 * i + 1] = -1.0f - position.z / (WORLD_SIZE * 2.0f);
	}
	return positions;
}

void BuildingHolder::DrawBounds() const
{
	BOXva->Bind();
	BOXvb->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < buildings.size(); i++)
	{
		buildings[i]->DrawBounds();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool BuildingHolder::CheckMovingBuilding_BuildingColision()
{
	ColisionBox* movingBuildingColisionBox = this->movingBuilding->GetColisionBox();

	for (int i = 0; i < this->buildings.size(); i++)
	{
		if (this->buildings[i]->CheckColision_AABB(movingBuildingColisionBox))
			return true;
	}
	return false;
}

bool BuildingHolder::CheckMovingBuilding_IFPointsColision(std::vector<ColisionBox*> IFcolisionBoxes)
{
	ColisionBox* movingBuildingColisionBox = this->movingBuilding->GetColisionBox();

	for (int i = 0; i < IFcolisionBoxes.size(); i++)
	{
		if (IFcolisionBoxes[i]->CheckColision_AABB(movingBuildingColisionBox))
			return true;
	}
	return false;
}

bool BuildingHolder::CheckMovingBuilding_AnimalColision(std::vector<ColisionBox*> animalColisions)
{
	ColisionBox* movingBuildingColisionBox = this->movingBuilding->GetColisionBox();

	for (int i = 0; i < animalColisions.size(); i++)
	{
		if (animalColisions[i]->CheckColision_SAT(movingBuildingColisionBox) && movingBuildingColisionBox->CheckColision_SAT(animalColisions[i]))
			return true;
	}
	return false;
}

bool BuildingHolder::CheckMovingBuilding_OutOfBounds(float size)
{
	glm::vec3 pos = this->movingBuilding->GetPosition();
	ColisionBox *col = this->movingBuilding->GetColisionBox();
	float lx = col->lengthX / 2.0f;
	float lz = col->lengthZ / 2.0f;
	if (pos.x >=  -lx|| pos.x <= -size +  lx|| pos.z >= -lz || pos.z <= -size + lz)
		return true;

	return false;
}
