#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;

void main(void)
{
	gl_Position = model * vec4(positions, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;

void main(void)
{
	color = texture(tex, TexCoords);
}