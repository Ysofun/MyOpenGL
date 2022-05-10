#vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCroods;

out vec2 TexCroods;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
	TexCroods = aTexCroods;
}

#fragment shader
#version 330 core

in vec2 TexCroods;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCroods);
}