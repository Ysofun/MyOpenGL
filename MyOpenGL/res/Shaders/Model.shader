#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCroods;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCroods;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCroods = aTexCroods;
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
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

	float shininess;
};
uniform Material u_Material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight u_DirLight;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};
uniform PointLight u_PointLight;

struct SpotLight
{
	vec3 direction;
	vec3 position;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};
uniform SpotLight u_SpotLight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCroods;

out vec4 Color;

uniform vec3 u_ViewPos;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), u_Material.shininess);

	vec3 ambient, diffuse, specular;
	for (int i = 0; i < u_Material.texture_diffuse_num; i++)
	{
		ambient += light.ambient * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
		diffuse += diff * light.diffuse * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
	}

	for (int i = 0; i < u_Material.texture_specular_num; i++)
	{
		specular += spec * light.specular * vec3(texture(u_Material.texture_specular[i], TexCroods));
	}

	return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), u_Material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * distance * distance);

	vec3 ambient, diffuse, specular;
	for (int i = 0; i < u_Material.texture_diffuse_num; i++)
	{
		ambient += light.ambient * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
		diffuse += diff * light.diffuse * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
	}

	for (int i = 0; i < u_Material.texture_specular_num; i++)
	{
		specular += spec * light.specular * vec3(texture(u_Material.texture_specular[i], TexCroods));
	}

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), u_Material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient, diffuse, specular;
	for (int i = 0; i < u_Material.texture_diffuse_num; i++)
	{
		ambient += light.ambient * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
		diffuse += diff * light.diffuse * vec3(texture(u_Material.texture_diffuse[i], TexCroods));
	}

	for (int i = 0; i < u_Material.texture_specular_num; i++)
	{
		specular += spec * light.specular * vec3(texture(u_Material.texture_specular[i], TexCroods));
	}

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	vec3 result = CalDirLight(u_DirLight, normal, viewDir);
	result += CalPointLight(u_PointLight, normal, FragPos, viewDir);
	result += CalSpotLight(u_SpotLight, normal, FragPos, viewDir);

	Color = vec4(result, 1.0f);
}

#shader geometry