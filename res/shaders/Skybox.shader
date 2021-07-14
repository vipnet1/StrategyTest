#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(void)
{
	vec4 pos = projection * view * model * vec4(positions, 1.0f);
	gl_Position = pos.xyww;
	TexCoords = positions;
}


#shader fragment
#version 330 core

const float lowerLimit = 0.0f;
const float upperLimit = 0.6f;

out vec4 color;

in vec3 TexCoords;

uniform samplerCube skyboxDay;
uniform samplerCube skyboxNight;

uniform vec3 skyColor;

uniform float nightIntensity;

void main(void)
{
	float factor = (TexCoords.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0f, 1.0f);
	vec4 skyBoxColor = mix(texture(skyboxDay, TexCoords), texture(skyboxNight, TexCoords), nightIntensity);
	color = mix(vec4(skyColor, 1.0f), skyBoxColor, factor);
}