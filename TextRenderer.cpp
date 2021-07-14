#include "TextRenderer.h"

TextRenderer::TextRenderer(unsigned int texUnit, float Ww, float Wh)
{
	projection = glm::ortho(0.0f, Ww, 0.0f, Wh);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48); //0-calculating the width base on the height automaticly


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned int c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		Texture* texture = new Texture(face, 0);
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	va = new VertexArray;
	vb = new VertexBuffer(sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	layout = new VertexBufferLayout;
	layout->Push<float>(4);
	va->AddBuffer(*vb, *layout);
	shader = new Shader("res/shaders/CH.shader");
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) const
{
	va->Bind();
	shader->Bind();
	shader->SetUniformMatrix4fv("projection", projection);
	shader->SetUniformVec3("textColor", color);
	shader->SetUniform1i("text", 0);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// Update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		ch.texture->Bind();
		vb->Bind();
		vb->UpdateData(vertices, sizeof(vertices));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6)* scale;
	}
}
