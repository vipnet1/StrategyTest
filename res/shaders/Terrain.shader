#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 aCords;
layout(location = 2) in vec3 aNormals;

out vec2 outCords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * view * model * vec4(positions.x, positions.y, positions.z, 1.0f);
	outCords = aCords;
	normal = mat3(transpose(inverse(model))) * aNormals;
}

#shader fragment
#version 330 core

in vec2 outCords;
out vec4 color;
in vec3 normal;


uniform sampler2D backgroundTexture;
uniform sampler2D textureRed;
uniform sampler2D textureGreen;
uniform sampler2D textureBlue;
uniform sampler2D textureBlend;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
};

uniform DirLight light;

uniform vec3 viewPos;

uniform float cordsTilingFactor;//should be of world size

void main(void)
{
	vec4 blendMapColour = texture(textureBlend, outCords);
	float backTextureAmount = 1.0f - (blendMapColour.r + blendMapColour.g + blendMapColour.b);

	vec2 tiledCords = outCords * cordsTilingFactor;

	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCords) * backTextureAmount;

	vec4 rTextureColour = texture(textureRed, tiledCords) * blendMapColour.r;
	vec4 gTextureColour = texture(textureGreen, tiledCords) * blendMapColour.g;
	vec4 bTextureColour = texture(textureBlue, tiledCords) * blendMapColour.b;

	vec4 mixedPixel = backgroundTextureColor + rTextureColour + gTextureColour + bTextureColour;

	
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f); //brightness
	
	vec3 ambient = light.ambient * vec3(mixedPixel);
	vec3 diffuse = light.diffuse * diff * vec3(mixedPixel);

	color = vec4((ambient + diffuse), 1.0f);
}