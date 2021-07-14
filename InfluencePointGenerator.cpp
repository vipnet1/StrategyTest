#include "InfluencePointGenerator.h"


InfluencePointGenerator::InfluencePointGenerator(int nrInfluencePoints, int spawnZone, Model* treeModel, Model* holyTreeModel, Terrain* terrain, float Wwidth, float Wheight, const ScoreHolder* scoreHolder, VertexArray* BOXva, VertexBuffer* BOXvb, const Shader* BOXshader, const Shader* IFshader)
	:BOXshader(BOXshader), IFshader(IFshader)
{
	this->BARshader = new Shader("res/shaders/InfluencePointBar.shader");

	this->BOXva = BOXva;
	this->BOXvb = BOXvb;

	for (int i = 0; i < nrInfluencePoints; i++)
	{
		float posx = -(rand() % spawnZone);
		float posz = -(rand() % spawnZone);
		this->IFpoints.push_back(new InfluencePoint(posx, posz, treeModel, holyTreeModel, terrain, scoreHolder, this->IFshader, this->BOXshader, this->BARshader));
	}

	this->BARva = new VertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(2);

	float vertices[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	this->BARvb = new VertexBuffer(sizeof(float) * 8, vertices);
	this->BARva->AddBuffer(*BARvb, layout);

	unsigned char indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	BARib = new IndexBuffer(sizeof(indices), indices);
	BARib->Bind();
}

void InfluencePointGenerator::DrawInfluencePoints() const
{
	for (int i = 0; i < this->IFpoints.size(); i++)
	{
		this->IFpoints[i]->Draw();
	}

	BARva->Bind();
	BARshader->Bind();
	BARib->Bind();

	float posy = 0.8f;

	for (int i = 0; i < this->IFpoints.size(); i++)
	{
		this->IFpoints[i]->DrawBar(glm::vec3(-0.85f, posy, 0.0f));
		posy -= 0.05f;
	}
}

void InfluencePointGenerator::CaptureInfluencePoints(std::vector<glm::vec3> turtlePositions, float deltaTime)
{
	for (int i = 0; i < this->IFpoints.size(); i++)
	{
		this->IFpoints[i]->Capture(turtlePositions, deltaTime);
	}
}


void InfluencePointGenerator::DrawBounds() const
{
	BOXva->Bind();
	BOXvb->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < this->IFpoints.size(); i++)
	{
		this->IFpoints[i]->DrawBounds();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

std::vector<ColisionBox*> InfluencePointGenerator::GetIFColisionBoxes()
{
	std::vector<ColisionBox*> colisionBoxes;
	for (int i = 0; i < this->IFpoints.size(); i++)
	{
		colisionBoxes.push_back(this->IFpoints[i]->GetColisionBox());
	}
	return colisionBoxes;
}


float* InfluencePointGenerator::GetIFPositionsNormalized(float heightToWidthRatio)
{
	int nrIFpoints = this->IFpoints.size();

	float* positions = new float[nrIFpoints * sizeof(float) * 2];
	for (int i = 0; i < nrIFpoints; i++)
	{
		glm::vec3 position = this->IFpoints[i]->GetPosition();
		positions[2 * i] = 1.0f + position.x / (WORLD_SIZE * 2.0f / heightToWidthRatio);
		positions[2 * i + 1] = -1.0f - position.z / (WORLD_SIZE * 2.0f);
	}
	return positions;
}