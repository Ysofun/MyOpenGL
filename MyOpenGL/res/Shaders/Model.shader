#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCroods;

out vec2 TexCroods;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCroods = aTexCroods;
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core
#define MAX_TEXTURE_NUM 8

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

uniform Material material;

in vec2 TexCroods;

out vec4 Color;

void main()
{
	Color = vec4(0.0f);
	for (int i = 0; i < material.texture_diffuse_num; i++)
	{
		Color += texture(material.texture_diffuse[i], TexCroods);
	}
	Color.a = 1.0f;
}