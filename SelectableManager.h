#pragma once

#include "AnimalSelectable.h"
#include "BuildingSelectable.h"

struct ImgSelectedData
{
	int ImgSelected;
	bool animalSelectedNow;
};

class SelectableManager
{
private:
	AnimalSelectable* animalSelectable;
	BuildingSelectable* buildingSelectable;

	bool animalSelectedNow;

public:
	SelectableManager(float locXNormalized, float locYNormalized, TextRenderer* tr, ScoreHolder* scoreHolder, float WidthFactor, float HeightFactor, Cave* cave, const BuildingHolder* buildingHolder);

	int GetSelectableChosenArea(float cursorXPosNormalized, float cursorYPosNormalized); //-1 - not on selectable, other values by location
	void Draw();
	inline glm::vec2 GetSelectableEnding() { return glm::vec2(0.0f, -0.8f); }
	int GetImageCount();
	void SwapSelectables();
	ImgSelectedData FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized);

	void RefundCostOfBuilding(BuildingType type);
};

