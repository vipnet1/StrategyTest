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

uniform int drawingSelected; //0 - not selected, 1 - selected, 2 - cameraPos on map

void main(void)
{
	if(drawingSelected == 1)
		color = vec4(1.0f);
	else if(drawingSelected == 2)
		color = vec4(1.0f, 0.0f, 1.0f, 0.25f);
	else if(drawingSelected == 3)
		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	else
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}