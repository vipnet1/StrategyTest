#pragma once
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define VERTEX_COUNT 64 //should be size of heightMap
#define WORLD_SIZE 128.0f


struct RenderData
{
	VertexBuffer* vb;
	IndexBuffer* ib;
};

static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

class Terrain
{
private:
	float gx;
	float gz;
	int verticesCount;
	float **heights;

	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Shader* shader;
	Texture* black, * red, * green, * blue, * blend;

	RenderData genVertices(const char* heightMapPath);
	float getVertexHeight(int x, int z, unsigned char* data, int nrChannels);
	glm::vec3 calculateNormal(int x, int z, unsigned char* data, int nrChannels);

public:
	Terrain(float x, float z, const char* heightMapPath, const char* texBlack, const char* texRed, const char* texGreen, const char* texBlue, const char* texBlend);
	inline Shader* getShader() { return shader; }
	inline int getVerticesCount() const { return verticesCount; }
	float getHeightOfTerrain(float worldX, float worldZ) const;

	void Bind() const;
	void SetDirLightProperties(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff);
};

