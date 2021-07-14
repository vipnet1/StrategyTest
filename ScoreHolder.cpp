#include "ScoreHolder.h"

ScoreHolder::ScoreHolder(int initBES, double initIncomeBESPerSecond)
{
	this->blueEssence = initBES;
	this->initIncomeBESPerSecond = initIncomeBESPerSecond;
	this->nrIFpointsTaken = 0;
}

void ScoreHolder::UpdateEssenceValue(float deltaTime)
{
	this->blueEssence += deltaTime * (this->initIncomeBESPerSecond + this->nrIFpointsTaken * INCOME_IF_POINT);
}

void ScoreHolder::setBlueEssence(double blueEssence)
{
	this->blueEssence = blueEssence;
}

void ScoreHolder::incNrIfPointsTaken() const
{
	this->nrIFpointsTaken++;
}

bool ScoreHolder::TryToBuy(unsigned int cost)
{
	if (this->blueEssence - cost > 0.0f)
	{
		this->blueEssence -= cost;
		return true;
	}
	else
		return false;
}
