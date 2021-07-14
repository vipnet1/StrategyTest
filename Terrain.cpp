#include "Terrain.h"
#include<GL/glew.h>

#include "stb_image.h"

#define MAX_HEIGHT 10.0f



Terrain::Terrain(float x, float z, const char* heightMapPath, const char* texBlack, const char* texRed, const char* texGreen, const char* texBlue, const char* texBlend)
	:gx(x), gz(z)
{
	va = new VertexArray();
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);

	RenderData rd = genVertices(heightMapPath);
	vb = rd.vb;
	ib = rd.ib;

	va->AddBuffer(*vb, layout);

	shader = new Shader("res/shaders/Terrain.shader");
	black = new Texture(texBlack, 0);
	red = new Texture(texRed, 1);
	green = new Texture(texGreen, 2);
	blue = new Texture(texBlue, 3);
	blend = new Texture(texBlend, 4);


	shader->Bind();
	shader->SetUniform1i("backgroundTexture", 0);
	shader->SetUniform1i("textureRed", 1);
	shader->SetUniform1i("textureGreen", 2);
	shader->SetUniform1i("textureBlue", 3);
	shader->SetUniform1i("textureBlend", 4);
}


RenderData Terrain::genVertices(const char* heightMapPath)
{

	int width, h, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(heightMapPath, &width, &h, &nrChannels, 0);

	std::cout << nrChannels << std::endl;

	const unsigned int count = VERTEX_COUNT * VERTEX_COUNT;
	verticesCount = count * 8;
	float* vertices = new float[count * 8];
	unsigned int *indices = new unsigned int[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];

	heights = new float*[VERTEX_COUNT];

	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		heights[i] = new float[VERTEX_COUNT];
	}

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			vertices[vertexPointer * 8] = -(float)j / (VERTEX_COUNT - 1) * WORLD_SIZE + gx;
			float height = getVertexHeight(j, i, data, nrChannels) - MAX_HEIGHT / 2.0f - 0.5f;
			vertices[vertexPointer * 8 + 1] = height;
			heights[j][i] = height;
			vertices[vertexPointer * 8 + 2] = -(float)i / (VERTEX_COUNT - 1) * WORLD_SIZE + gz;
			vertices[vertexPointer * 8 + 3] = (float)j / (VERTEX_COUNT - 1);
			vertices[vertexPointer * 8 + 4] = (float)i / (VERTEX_COUNT - 1);
			//normals
			glm::vec3 normal = calculateNormal(j, i, data, nrChannels);
			vertices[vertexPointer * 8 + 5] = normal.x;
			vertices[vertexPointer * 8 + 6] = normal.y;
			vertices[vertexPointer * 8 + 7] = normal.z;
			vertexPointer++;
		}
	}

	stbi_image_free(data);

	int pointer = 0;
	for (int z = 0; z < VERTEX_COUNT - 1; z++)
	{
		for (int x = 0; x < VERTEX_COUNT - 1; x++)
		{
			int topLeft = (z * VERTEX_COUNT) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * VERTEX_COUNT) + x;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	return { new VertexBuffer(sizeof(float) * count * 8, vertices, GL_STATIC_DRAW), new IndexBuffer(sizeof(unsigned int) * 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1), indices, GL_STATIC_DRAW) };

}

float Terrain::getHeightOfTerrain(float worldX, float worldZ) const
{
	float terrainX = this->gx - worldX;
	float terrainZ = this->gz - worldZ;
	float gridSquareSize = WORLD_SIZE / (VERTEX_COUNT - 1.0f);
	int gridX = (int)floor(terrainX / gridSquareSize);
	int gridZ = (int)floor(terrainZ / gridSquareSize);
	if (gridX >= VERTEX_COUNT - 1.0f || gridX < 0 || gridZ >= VERTEX_COUNT - 1.0f || gridZ < 0)
		return 0;

	float xCoord = std::fmod(terrainX, gridSquareSize) / gridSquareSize;
	float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;
	float answer;

	if (xCoord <= (1 - zCoord))
	{
		answer = barryCentric(glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ], 0), glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	else
	{
		answer = barryCentric(glm::vec3(1, heights[gridX + 1][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ + 1], 1), glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}

	return answer + 0.5f;
}

void Terrain::Bind() const
{
	va->Bind();
	vb->Bind();
	ib->Bind();
	shader->Bind();

	glActiveTexture(GL_TEXTURE0);
	black->Bind();
	glActiveTexture(GL_TEXTURE1);
	red->Bind();
	glActiveTexture(GL_TEXTURE2);
	green->Bind();
	glActiveTexture(GL_TEXTURE3);
	blue->Bind();
	glActiveTexture(GL_TEXTURE4);
	blend->Bind();
}

void Terrain::SetDirLightProperties(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff)
{
	shader->Bind();
	shader->SetUniformVec3("light.direction", dir);
	shader->SetUniformVec3("light.ambient", ambient);
	shader->SetUniformVec3("light.diffuse", diff);
}


float Terrain::getVertexHeight(int x, int z, unsigned char* data, int nrChannels)
{
	if (x >= VERTEX_COUNT - 1.0f || x < 0 || z >= VERTEX_COUNT - 1.0f || z < 0)
		return 0;

	float r = data[z * VERTEX_COUNT * nrChannels + x * nrChannels];
	r /= 255.0f;
	r *= MAX_HEIGHT;

	return r;
}

glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char* data, int nrChannels)
{
	float heightL = getVertexHeight(x - 1, z, data, nrChannels);
	float heightR = getVertexHeight(x + 1, z, data, nrChannels);
	float heightD = getVertexHeight(x, z - 1, data, nrChannels);
	float heightU = getVertexHeight(x, z + 1, data, nrChannels);

	return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
}
