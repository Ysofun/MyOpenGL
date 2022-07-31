#include "Shader.h"
#include "ErrorCatch.h"

#include <glad/glad.h>

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath)
{
	ShaderProgramSource source(ParseShader(filepath));
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, float value) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniform1f(location, value));
}

void Shader::SetUniformVec2(const std::string& name, float v1, float v2) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniform2f(location, v1, v2));
}

void Shader::SetUniformVec2(const std::string& name, glm::vec2 vector2) const
{
	SetUniformVec2(name, vector2.x, vector2.y);
}

void Shader::SetUniformVec3(const std::string& name, float v1, float v2, float v3) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniform3f(location, v1, v2, v3));
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 vector3) const
{
	SetUniformVec3(name, vector3.x, vector3.y, vector3.z);
}

void Shader::SetUniformVec4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniform4f(location, v1, v2, v3, v4));
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 matrix) const
{
	int location = GetUniformLocation(name);
	if (location != -1)
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
	};

	std::string line;
	std::stringstream ss[3];
	ss[0].str("");
	ss[1].str("");
	ss[2].str("");
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("geometry") != std::string::npos)
				type = ShaderType::GEOMETRY;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Fail to compile " << type << " shader!!!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& geometrySource, 
	const std::string& fragmentSource)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs, gs, fs;
	if (!vertexSource.empty())
	{
		vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLCall(glAttachShader(program, vs));
	}
	if (!geometrySource.empty())
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometrySource);
		GLCall(glAttachShader(program, gs));
	}
		
	if (!fragmentSource.empty())
	{
		fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		GLCall(glAttachShader(program, fs));
	}
		
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	if (!vertexSource.empty())
	{
		GLCall(glDeleteShader(vs));
	}
	if (!geometrySource.empty())
	{
		GLCall(glDeleteShader(gs));
	}
	if (!fragmentSource.empty())
	{
		GLCall(glDeleteShader(fs));
	}

	return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Uniform " << name << " doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
