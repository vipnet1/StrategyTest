#pragma once
class FrameBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int textureID, renderbufferID;
	bool texBound, renderBound;

public:
	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	void AttachTexture(unsigned int width, unsigned int height);
	void AttachRenderbuffer(unsigned int width, unsigned int height);

	inline unsigned int GetTextureId() { return textureID; }
};

