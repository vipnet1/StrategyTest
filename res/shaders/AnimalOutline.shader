#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;


uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main(void)
{
	gl_Position = projection * view * model * vec4(positions, 1.0f);
}


#shader fragment
#version 330 core

out vec4 color;


void main(void)
{
	color = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}