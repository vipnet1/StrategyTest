#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex;

uniform mat4 projection;

out vec2 TexCoords;

void main(void)
{
	gl_Position = projection * vec4(vertex.xy, 0.0f ,1.0f);
	TexCoords = vertex.zw;
}


#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 t = texture(text, TexCoords);
	if (t.r < 0.25f)
		discard;
	vec4 sampled = vec4(1.0, 1.0, 1.0, t.r);
	color = vec4(textColor, 1.0) * sampled;
}