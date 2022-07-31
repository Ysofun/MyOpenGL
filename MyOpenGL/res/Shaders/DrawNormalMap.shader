#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
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
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBitangent);
	vec3 N = normalize(normalMatrix * aNormal);
	
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

uniform bool u_NormalMapping;

void main()
{
	vec3 normal = texture(u_NormalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(u_DiffuseMap, fs_in.TexCoords).rgb;
	vec3 ambient = 0.1 * color;
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfWayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfWayDir, normal), 0.0), 32.0);
	vec3 specular = spec * vec3(0.2);

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}