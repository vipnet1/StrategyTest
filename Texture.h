#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>

class Texture
{
private:
	unsigned int m_RendererID;
	unsigned int textureUnit;

public:
	Texture(const char* path, unsigned int texUnit);
	Texture(FT_Face face, unsigned int texUnit);
	Texture(unsigned int textureId, unsigned int texUnit);
	~Texture();

	void Bind() const;
	void Unbind() const;
};

