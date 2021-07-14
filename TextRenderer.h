#pragma once

#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>
#define FT_FREETYPE_H

#include<map>

struct Character
{
	Texture* texture;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer
{
private:
	mutable std::map<char, Character> Characters;
	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* layout;
	Shader* shader;
	glm::mat4 projection;

public:
	TextRenderer(unsigned int texUnit, float Ww, float Wh);
	const inline Shader* GetCharacterShader() { return shader; };
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color) const;
};
