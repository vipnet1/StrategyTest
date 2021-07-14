#include "Selectable.h"

#define MAX_SELECTABLE_IMAGES 8


Selectable::Selectable(float locXNormalized, float locYNormalized, TextRenderer* tr, glm::vec2 selectableEnding)
{
	this->locXNormalized = locXNormalized;
	this->locYNormalized = locYNormalized;
	this->tr = tr;
	this->selectableEnding = selectableEnding;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f,  1.0f, 0.0f,
		0.5f, 0.5f,   1.0f, 1.0f,
		-0.5f, 0.5f,  0.0f, 1.0f
	};

	va = new VertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vb = new VertexBuffer(sizeof(vertices), vertices);

	va->AddBuffer(*vb, layout);
	shader = new Shader("res/shaders/Selectable.shader");

	unsigned char indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	ib = new IndexBuffer(sizeof(indices), indices);
	shader->Bind();
	shader->SetUniform1i("tex", 0);
}

int Selectable::GetSelectableChosenArea(float cursorXPosNormalized, float cursorYPosNormalized) //-1 not on selectable, -2 - swap selectables
{
	glm::vec2 selectableEnding = this->GetSelectableEnding();
	if (cursorXPosNormalized < selectableEnding.x && cursorYPosNormalized < selectableEnding.y)
	{
		for (int i = 1; i <= this->images.size(); i++)
		{
			if (cursorXPosNormalized < -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * i)) && cursorXPosNormalized > -(1.0f - (1.0f / MAX_SELECTABLE_IMAGES * (i - 1))) && cursorYPosNormalized < selectableEnding.y)
			{
				return i - 1;
			}
		}
	}
	return -1;
}
