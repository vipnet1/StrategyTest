#pragma once

#define INCOME_IF_POINT 0.5f

class ScoreHolder
{
private:
	double blueEssence;
	double initIncomeBESPerSecond;

	mutable int nrIFpointsTaken;

public:
	ScoreHolder(int initBES, double initIncomeBESPerSecond);
	void UpdateEssenceValue(float deltaTime);

	inline double GetBlueEssence() const { return blueEssence; }
	inline float GetBlueEssenceIncome() const { return initIncomeBESPerSecond + nrIFpointsTaken * INCOME_IF_POINT; }
	void setBlueEssence(double blueEssence);

	void incNrIfPointsTaken() const;
	bool TryToBuy(unsigned int cost); //returns true if bought, false else
};