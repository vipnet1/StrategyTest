#pragma once

#include "InfluencePoint.h"

class InfluencePointGenerator
{
private:
	std::vector<InfluencePoint*> IFpoints;

	VertexArray* BARva;
	VertexBuffer* BARvb;
	Shader* BARshader;
	IndexBuffer* BARib;

	VertexArray* BOXva;
	VertexBuffer* BOXvb;

	const Shader* BOXshader;
	const Shader* IFshader;

public:
	InfluencePointGenerator(int nrInfluencePoints, int spawnZone, Model* treeModel, Model* holyTreeModel, Terrain* terrain, float Wwidth, float Wheight, const ScoreHolder* scoreHolder, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* IFshader);

	void DrawInfluencePoints() const;
	void CaptureInfluencePoints(std::vector<glm::vec3> turtlePositions, float deltaTime);

	inline const Shader* GetBarShader() { return BARshader; }
	void DrawBounds() const;

	std::vector<ColisionBox*> GetIFColisionBoxes();
	float* GetIFPositionsNormalized(float heightToWidthRatio);

	inline unsigned int GetNumberOfIF() { return this->IFpoints.size() * sizeof(float) * 2; }
};

