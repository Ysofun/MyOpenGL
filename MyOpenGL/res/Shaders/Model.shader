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

in vec2 TexCroods;

out vec4 Color;

uniform sampler2D texture_diffuse;

void main()
{
	Color = texture(texture_diffuse, TexCroods);
}