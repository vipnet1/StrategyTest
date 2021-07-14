#include "Map.h"

Map::Map(float userResolutionWidth, float userResolutionHeight)
{

	this->userResolutionWidth = userResolutionWidth;
	this->userResolutionHeight = userResolutionHeight;

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
	shader = new Shader("res/shaders/MainMap.shader");

	unsigned char indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	ib = new IndexBuffer(sizeof(indices), indices);
	mapTexture = new Texture("res/textures/maps/mainMap.jpg", 0);
	shader->Bind();
	shader->SetUniform1i("tex", 0);

	this->locYNormalized = -0.5f;
	this->locXNormalized = 1.0f - (0.5f * userResolutionHeight / userResolutionWidth);

	Pva = new VertexArray;
	VertexBufferLayout Playout;
	Playout.Push<float>(2);
	Pvb = new VertexBuffer(16, NULL, GL_DYNAMIC_DRAW);
	Pva->AddBuffer(*Pvb, Playout);
	Pshader = new Shader("res/shaders/Point.shader");
}

void Map::Draw(float* animalPositionsNotSelectedNormalized, unsigned int notSelectedSize, float* animalPositionsSelectedNormalized, unsigned int selectedSize, float* buildingPositionsNormalized, unsigned int buildingSize, float* IFPositionsNormalized, unsigned int IFSize, float* cameraPositionNormalized)
{
	va->Bind();
	shader->Bind();
	ib->Bind();
	glm::mat4 model(1.0f);
	glm::mat4 trans = glm::translate(model, glm::vec3(1.0f - (userResolutionHeight / userResolutionWidth) / 2.0f, -0.75f, 0.0f));
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f * (userResolutionHeight / userResolutionWidth), 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3((userResolutionHeight / userResolutionWidth) * 0.5f, 0.5f, 1.0f));
	model = trans2 * trans * model;
	shader->SetUniformMatrix4fv("model", model);
	mapTexture->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	ib->Unbind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	Pva->Bind();
	Pvb->Bind();

	glPointSize(5.0f);

	Pvb->UpdateData(animalPositionsNotSelectedNormalized, notSelectedSize);
	Pshader->Bind();
	Pshader->SetUniform1i("drawingSelected", 0);
	glDrawArrays(GL_POINTS, 0, notSelectedSize / (sizeof(float) * 2));

	Pvb->UpdateData(buildingPositionsNormalized, buildingSize);
	glDrawArrays(GL_POINTS, 0, buildingSize / (sizeof(float) * 2));

	Pshader->SetUniform1i("drawingSelected", 1);
	Pvb->UpdateData(animalPositionsSelectedNormalized, selectedSize);
	glDrawArrays(GL_POINTS, 0, selectedSize / (sizeof(float) * 2));

	Pshader->SetUniform1i("drawingSelected", 3);
	Pvb->UpdateData(IFPositionsNormalized, IFSize);
	glDrawArrays(GL_POINTS, 0, IFSize / (sizeof(float) * 2));

	glPointSize(20.0f);

	Pshader->SetUniform1i("drawingSelected", 2);
	Pvb->UpdateData(cameraPositionNormalized, sizeof(float) * 2);
	glDrawArrays(GL_POINTS, 0, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

glm::vec3 Map::processMouseInput(float locXNormalized, float locYNormalized)
{
	glm::vec3 pos;
	pos.x = -(1.0f - locXNormalized) * 2.0f * (this->userResolutionWidth / this->userResolutionHeight);
	pos.y = 0.0f;
	pos.z = (-1.0f - locYNormalized) * 2.0f;
	return pos;
}
