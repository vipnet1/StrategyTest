#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;


void main(void)
{
	gl_Position = vec4(positions, 0.0f, 1.0f);
}


#shader fragment
#version 330 core

out vec4 color;

void main(void)
{
	color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}