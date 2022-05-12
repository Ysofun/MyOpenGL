#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	vs_out.FragPos = aPos;
	vs_out.Normal = aNormal;
	vs_out.TexCoords = aTexCoords;
	gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
}

#fragment shader
#version 330 core

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_FloorTexture;

uniform vec3 u_ViewPos;
uniform vec3 u_LightPos[4];
uniform vec3 u_LightColor[4];
uniform bool u_Gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 halfWayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfWayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (u_Gamma ? distance * distance : distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

void main()
{
	vec3 color = texture(u_FloorTexture, fs_in.TexCoords).rgb;
	vec3 lighting = vec3(0.0);
	for (int i = 0; i < 4; i++)
	{
		lighting += BlinnPhong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPos[i], u_LightColor[i]);
	}
	color *= lighting;
	if (u_Gamma)
	{
		color = pow(color, vec3(1.0 / 2.2));
	}
	FragColor = vec4(color, 1.0);
}