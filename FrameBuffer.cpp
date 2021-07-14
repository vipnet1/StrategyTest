#include "FrameBuffer.h"
#include<GL/glew.h>
#include<iostream>

FrameBuffer::FrameBuffer()
	:m_RendererID(0), textureID(0), renderbufferID(0)
{
	this->texBound = false;
	this->renderBound = false;

	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	int checkAttachment = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (checkAttachment != GL_FRAMEBUFFER_COMPLETE && checkAttachment != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		std::cout << "Can't create a Framebuffer" << std::endl;

	this->Unbind();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AttachTexture(unsigned int width, unsigned int height)
{
	if (this->texBound)
	{
		std::cout << "The texture(FrameBuffer) is Set already" << std::endl;
		return;
	}

	this->Bind();

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	this->textureID = texture;
	this->texBound = true;
	this->Unbind();
}

void FrameBuffer::AttachRenderbuffer(unsigned int width, unsigned int height)
{
	if (this->renderBound)
	{
		std::cout << "Renderuffer(FrameBuffer) is Set already" << std::endl;
		return;
	}

	this->Bind();
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	this->renderbufferID = rbo;
	this->renderBound = true;
	this->Unbind();
}
