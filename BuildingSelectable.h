#pragma once
#include "Selectable.h"
#include "ScoreHolder.h"

#include "BuildingHolder.h"

enum class ImgBuilding
{
	NONE = -1, IMG_TARGET = 0, IMG_PYRAMID = 1, IMG_STATUE = 2, IMG_PARROT = 3
};

class BuildingSelectable : public Selectable
{
private:
	ScoreHolder* scoreHolder;
	float* buildingCost;

	const float widthFactor, heightFactor;
	const BuildingHolder* buildingHolder;

public:
	BuildingSelectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding, ScoreHolder* scoreHolder, float widthFactor, float heightFactor, const BuildingHolder* buildingHolder);
	ImgBuilding FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized);

	void Draw();
	void RefundCostOf(BuildingType type);
};

