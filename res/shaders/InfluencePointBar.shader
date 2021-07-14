#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;

uniform int blue;

void main(void)
{
	if(blue == 1)
		gl_Position = model * vec4(positions, -0.01f, 1.0f);
	else
		gl_Position = model * vec4(positions, 0.0f, 1.0f);
}


#shader fragment
#version 330 core

out vec4 color;

uniform int blue;
uniform int bold;

void main(void)
{
	if (blue == 1)
	{
		if(bold == 1)
			color = vec4(0.0f, 0.0f, 1.0f, 0.25f);
		else
			color = vec4(1.0f, 0.0f, 1.0f, 0.2f);
	}
	else
	{
		color = vec4(0.0f, 0.0f, 0.0f, 0.2f);
	}
}