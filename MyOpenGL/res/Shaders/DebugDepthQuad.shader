#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = vec4(aPos, 1.0);
}

#fragment shader
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_DepthMap;
uniform float u_NearPlane;
uniform float u_FarPlane;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}

void main()
{
	float depthValue = texture(u_DepthMap, TexCoords).r;
	FragColor = vec4(vec3(depthValue), 1.0);
}