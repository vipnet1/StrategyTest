#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main(void)
{
	gl_Position = vec4(positions, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}

#shader fragment

out vec4 color;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main(void)
{
	color = texture(screenTexture, TexCoords);
}