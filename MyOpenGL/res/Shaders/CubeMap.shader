#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	Position = vec3(u_Model * vec4(aPos, 1.0));
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#fragment shader
#version 330 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{
	float radio = 1.00 / 1.52;
	vec3 I = normalize(Position - u_CameraPos);
	vec3 refractor = refract(I, normalize(Normal), radio);
	vec3 reflector = reflect(I, normalize(Normal));
	vec4 col1 = vec4(texture(u_Skybox, refractor).rgb, 1.0);
	vec4 col2 = vec4(texture(u_Skybox, reflector).rgb, 1.0);
	FragColor = col1 * 0.8 + col2 * 0.2;
}