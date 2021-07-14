#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Terrain.h"

#define MY_RESOLUTION_WIDTH 1920.0f
#define MY_RESOLUTION_HEIGHT 1080.0f

class Map
{
private:
	VertexArray* va, *Pva;
	VertexBuffer* vb, *Pvb;
	IndexBuffer* ib;
	Texture* mapTexture;
	Shader* shader, *Pshader;

	float locXNormalized, locYNormalized;
	float userResolutionWidth, userResolutionHeight;

public:
	Map(float userResolutionWidth, float userResolutionHeight);
	void Draw(float* animalPositionsNotSelectedNormalized, unsigned int notSelectedSize, float* animalPositionsSelectedNormalized, unsigned int selectedSize, float* buildingPositionsNormalized, unsigned int buildingSize, float* IFPositionsNormalized, unsigned int IFSize, float* cameraPositionNormalized);

	glm::vec3 processMouseInput(float locXNormalized, float locYNormalized);
	inline bool isInside(float locXNormalized, float locYNormalized) { return locXNormalized > this->locXNormalized&& locYNormalized < this->locYNormalized; }
};

