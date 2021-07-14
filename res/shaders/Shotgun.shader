#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(void)
{
	gl_Position = projection * model * vec4(positions, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;


uniform sampler2D shotgunTexture;

void main(void)
{
	color = texture(shotgunTexture, TexCoords);
}