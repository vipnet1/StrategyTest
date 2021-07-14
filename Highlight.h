#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"

class Highlight
{
private:
	VertexArray *va;
	VertexBuffer* vb;
	IndexBuffer *ib;
	Shader* shader;

	glm::vec2 initialPointHighlighting;
	bool haveInitHL;
	float* currHighlight;

	float screenWidth, screenHeight;

public:
	Highlight(float scrWidth, float scrHeight);
	void processHighlight(float xpos, float ypos); //also binds everything automaticly and sets data
	void DisableInitHL();

	inline bool GetHaveInitHL() { return haveInitHL; }

	inline float* GetCurrHighlight() { return currHighlight; }
};

