#vertex shader
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
	vs_out.TexCoords = aTexCoords;
	vs_out.FragPos = vec3(u_Model * vec4(aPosition, 1.0));

	vec3 T = normalize(mat3(u_Model) * aTangent);
	vec3 B = normalize(mat3(u_Model) * aBitangent);
	vec3 N = normalize(mat3(u_Model) * aNormal);
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.TangentLightPos = TBN * u_LightPos;
	vs_out.TangentViewPos = TBN * u_ViewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
}

#fragment shader
#version 330 core

in VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_DepthMap;

uniform bool u_Parallax;
uniform float u_HeightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = texture(u_DepthMap, texCoords).r;
	return texCoords - viewDir.xy / viewDir.z * (height * u_HeightScale);
}

void main()
{
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = fs_in.TexCoords;
	if (u_Parallax)
	{
		texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
	}

	vec3 normal = texture(u_NormalMap, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(u_DiffuseMap, texCoords).rgb;
	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * color;

	vec3 halfWayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfWayDir), 0.0), 32.0);
	vec3 specular = spec * vec3(0.2);

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}