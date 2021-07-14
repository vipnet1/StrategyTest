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
	gl_Position = projection * view * model * vec4(positions, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;


struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
};

uniform Material material;

void main(void)
{
	color = texture(material.texture_diffuse1, TexCoords);
}