#include "Highlight.h"

Highlight::Highlight(float scrWidth, float scrHeight)
{
	this->screenWidth = scrWidth;
	this->screenHeight = scrHeight;
	this->va = new VertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	this->vb = new VertexBuffer(sizeof(float) * 8, NULL, GL_DYNAMIC_DRAW);
	this->va->AddBuffer(*vb, layout);
	this->shader = new Shader("res/shaders/ChooseSquare.shader");

	unsigned char indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	this->ib = new IndexBuffer(sizeof(unsigned char) * 8, indices, GL_STATIC_DRAW);
	this->haveInitHL = false;
}

void Highlight::processHighlight(float xpos, float ypos)
{
	if (!haveInitHL)
	{
		initialPointHighlighting.x = 2.0f * ((xpos) / this->screenWidth) - 1.0f;
		initialPointHighlighting.y = 2.0f * ((this->screenHeight - ypos) / this->screenHeight) - 1.0f;
		haveInitHL = true;
	}

	glm::vec2 currPointHighlighting;
	currPointHighlighting.x = 2.0f * ((xpos) / this->screenWidth) - 1.0f;
	currPointHighlighting.y = 2.0f * ((this->screenHeight - ypos) / this->screenHeight) - 1.0f;

	currHighlight = new float[8];
	currHighlight[0] = initialPointHighlighting.x;
	currHighlight[1] = initialPointHighlighting.y;
	currHighlight[2] = initialPointHighlighting.x;
	currHighlight[3] = currPointHighlighting.y;
	currHighlight[4] = currPointHighlighting.x;
	currHighlight[5] = currPointHighlighting.y;
	currHighlight[6] = currPointHighlighting.x;
	currHighlight[7] = initialPointHighlighting.y;

	va->Bind();
	vb->Bind();
	ib->Bind();
	shader->Bind();
	vb->UpdateData(currHighlight, sizeof(float) * 8);
}

void Highlight::DisableInitHL()
{
	this->haveInitHL = false;
}
