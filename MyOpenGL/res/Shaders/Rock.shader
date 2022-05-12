#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = u_Projection * u_View * aInstanceMatrix * vec4(aPos, 1.0);
}

#fragment shader
#version 330 core
#define MAX_TEXTURE_NUM 8

in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse[MAX_TEXTURE_NUM];
	sampler2D texture_specular[MAX_TEXTURE_NUM];
	sampler2D texture_normal[MAX_TEXTURE_NUM];
	sampler2D texture_height[MAX_TEXTURE_NUM];
	int texture_diffuse_num;
	int texture_specular_num;
	int texture_normal_num;
	int texture_height_num;
};
uniform Material u_Material;

void main()
{
	FragColor = texture(u_Material.texture_diffuse[0], TexCoords);
}