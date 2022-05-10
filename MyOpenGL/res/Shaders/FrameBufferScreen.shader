#vertex shader
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}

#fragment shader
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;

const float offset = 1.0 / 300.0;

void main()
{
	vec2 offset[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
		);

	float kernal[9] = float[](
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
		);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(texture1, TexCoords.st + offset[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		col += sampleTex[i] * kernal[i];
	}
	FragColor = vec4(col, 1.0f);
}