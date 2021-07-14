#pragma once

#include "Selectable.h"
#include "ScoreHolder.h"

enum class ImgAnimal
{
	NONE = -1, IMG_WOLF = 0, IMG_PANDA = 1, IMG_TURTLE = 2, IMG_HAMMER = 3
};



class AnimalSelectable : public Selectable
{
private:
	ScoreHolder* scoreHolder;
	float *animalCost;

	const float widthFactor, heightFactor;
	const Cave* cave;

public:
	AnimalSelectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding, ScoreHolder* scoreHolder, float widthFactor, float heightFactor, Cave* cave);
	ImgAnimal FindImageSelected(float cursorXPosNormalized, float cursorYPosNormalized);
	void Draw();
};

