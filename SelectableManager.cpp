#include "SelectableManager.h"

SelectableManager::SelectableManager(float locXNormalized, float locYNormalized, TextRenderer* tr, ScoreHolder* scoreHolder, float WidthFactor, float HeightFactor, Cave* cave, const BuildingHolder* buildingHolder)
{
	glm::vec2 selectableEnding = this->GetSelectableEnding();
	animalSelectable = new AnimalSelectable(locXNormalized, locYNormalized, tr, selectableEnding, scoreHolder, WidthFactor, HeightFactor, cave);
	buildingSelectable = new BuildingSelectable(locXNormalized, locYNormalized, tr, selectableEnding, scoreHolder, WidthFactor, HeightFactor, buildingHolder);
	this->animalSelectedNow = true;
}

int SelectableManager::GetSelectableChosenArea(float cursorXPosNormalized, float cursorYPosNormalized)
{
	if (this->animalSelectedNow)
		return animalSelectable->GetSelectableChosenArea(cursorXPosNormalized, cursorYPosNormalized);
	else
		return buildingSelectable->GetSelectableChosenArea(cursorXPosNormalized, cursorYPosNormalized);
}

void SelectableManager::Draw()
{
	if (this->animalSelectedNow)
		animalSelectable->Draw();
	else
		buildingSelectable->Draw();
}

int SelectableManager::GetImageCount()
{
	if (this->animalSelectedNow)
		return animalSelectable->GetImageCount();
	else
		return buildingSelectable->GetImageCount();
}

void SelectableManager::SwapSelectables()
{
	this->animalSelectedNow = !this->animalSelectedNow;
}

ImgSelectedData SelectableManager::FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized)
{
	if (this->animalSelectedNow)
		return { (int)animalSelectable->FindImageSelected(cursorXPosNormalized, cursorYPosNormalized) , this->animalSelectedNow};
	else
		return { (int)buildingSelectable->FindImageSelected(cursorXPosNormalized, cursorYPosNormalized) , this->animalSelectedNow};
}

void SelectableManager::RefundCostOfBuilding(BuildingType type)
{
	this->buildingSelectable->RefundCostOf(type);
}
