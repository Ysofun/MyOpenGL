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

struct Light
{
	vec3 position;
	//vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};
uniform Light light;

in vec2 TexCroods;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 u_ViewPos;

void main()
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));

	// ambient light
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCroods));

	// diffuse light
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCroods));

	// specular light
	vec3 viewDir = normalize(u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCroods));

	// point light attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// final result
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}

#shader geometry