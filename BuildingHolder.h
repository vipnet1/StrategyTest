#pragma once

#include "VertexArray.h"

#include "Target.h"
#include "Pyramid.h"
#include "Statue.h"
#include "ScoreHolder.h"

class BuildingHolder
{
private:
	VertexArray* BOXva;
	VertexBuffer* BOXvb;

	const Shader* BOXshader;
	const Shader* buildingShader;

	std::vector<Building*> buildings;
	Model* targetModel, *pyramidModel, *statueModel;
	bool buildingMovingNow;
	Building* movingBuilding;

public:
	BuildingHolder(Model* targetModel, Model* pyramidModel, Model* statueModel, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* buildingShader);

	void Draw(bool isMovingBuildingColiding) const;
	void AddMovingBuilding(BuildingType type);
	void MoveBuilding(float currXPos, float currZPos, float currHeight);
	void TryToPlaceMovingBuilding();
	bool TryToCancelMovingBuilding(); //true if canceled

	inline bool IsBuildingMovingNow() const { return buildingMovingNow; }
	BuildingType GetLastMovingBuildingType();

	float* GetBuildingPositionsNormalized(float heightToWidthRatio);
	inline unsigned int GetNumberOfBuildings() { return this->buildings.size() * sizeof(float) * 2; }

	void DrawBounds() const;

	bool CheckMovingBuilding_BuildingColision();
	bool CheckMovingBuilding_IFPointsColision(std::vector<ColisionBox*> IFcolisionBoxes);
	bool CheckMovingBuilding_AnimalColision(std::vector<ColisionBox*> animalColisions);
	bool CheckMovingBuilding_OutOfBounds(float size);
};

