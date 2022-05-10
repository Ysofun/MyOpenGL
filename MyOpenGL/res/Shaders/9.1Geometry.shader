#vertex shader
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out VS_OUT
{
	vec3 color;
} vs_out;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	vs_out.color = aColor;
}

#geometry shader
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT
{
	vec3 color;
} gs_in[];

out vec3 fColor;

void BuildHouse(vec4 position, vec3 color)
{
	fColor = color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	BuildHouse(gl_in[0].gl_Position, gs_in[0].color);
}

#fragment shader
#version 330 core

in vec3 fColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(fColor, 1.0);
}