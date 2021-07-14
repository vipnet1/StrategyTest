#pragma once
#include<GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int drawType;

public:
	VertexBuffer(unsigned int size, void* data, unsigned int drawType = GL_STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void UpdateData(const void* data, unsigned int size);
};

