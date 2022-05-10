#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCroods;

out vec2 TexCroods;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	TexCroods = aTexCroods;
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core
in vec2 TexCroods;
out vec4 Color;

uniform sampler2D u_Texture;

void main()
{
	Color = texture(u_Texture, TexCroods);
}

#shader geometry