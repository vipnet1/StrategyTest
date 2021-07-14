#pragma once
#include<GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_RendererID;

public:
	IndexBuffer(unsigned int size, void* data, unsigned int drawType = GL_STATIC_DRAW);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};

