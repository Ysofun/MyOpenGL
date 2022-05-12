#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#fragment shader
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}