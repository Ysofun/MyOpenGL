#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCroods;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCroods;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCroods = aTexCroods;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0f);
}

#shader fragment
#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

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
#define NR_POINT_LIGHTS 4
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
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

in vec2 TexCroods;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 u_ViewPos;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0f);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	// result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCroods));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCroods));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCroods));
	return (ambient + diffuse + specular);

}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCroods));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCroods));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCroods));
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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCroods));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCroods));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCroods));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main()
{
	// diffuse light
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	vec3 result = CalDirLight(u_DirLight, normal, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalPointLight(u_PointLights[i], normal, FragPos, viewDir);
	}

	color = vec4(result, 1.0f);
}

#shader geometry