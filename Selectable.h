#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "TextRenderer.h"
#include "Cave.h"

#define MAX_SELECTABLE_IMAGES 8


class Selectable
{
protected:
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Texture* mainTexture;
	std::vector<Texture*> images;//7 max to not exceed from the selectable(8 also better to not)
	Shader* shader;
	TextRenderer* tr;

	float locXNormalized, locYNormalized;

	glm::vec2 selectableEnding;

public:
	Selectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding);
	int GetSelectableChosenArea(float cursorXPosNormalized, float cursorYPosNormalized); //-1 - not on selectable, other values by location
	inline glm::vec2 GetSelectableEnding() { return glm::vec2(0.0f, -0.8f); }
	inline int GetImageCount() { return images.size(); }
};

