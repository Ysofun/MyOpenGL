#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
}

#shader fragment
#version 330 core

out vec4 color;

uniform vec3 u_ObjColor;
uniform vec3 u_LightColor;

void main()
{
	color = vec4(u_ObjColor * u_LightColor, 1.0f);
}