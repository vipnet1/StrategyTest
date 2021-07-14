#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "Terrain.h"
#include "ScoreHolder.h"

#include "ColisionBox.h"

class InfluencePoint
{
private:
	glm::vec3 position;
	Model* holyTreeModel, *treeModel;
	float currCaptureVal;
	bool captured;

	const float OnCaptureValue;
	const float CapturePerSecond;
	const float DecCapturePerSecond;

	const ScoreHolder* scoreHolder;

	ColisionBox* colisionBox;

	const Shader* IFshader;
	const Shader* BOXshader;
	const Shader* BARshader;

public:
	InfluencePoint(float posx, float posz, Model* treeModel, Model* holyTreeModel, const Terrain* terrain, const ScoreHolder* scoreHolder, const Shader* IFshader, const Shader* BOXshader, const Shader* BARshader);

	void Draw();
	void DrawBar(glm::vec3 pos);
	void Capture(std::vector<glm::vec3> turtlePosition, float deltaTime);

	void DrawBounds();

	inline ColisionBox* GetColisionBox() { return colisionBox; }

	inline glm::vec3 GetPosition() { return this->position; }
};

