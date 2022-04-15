#include "Shader.h"

#include <ErrorCatch.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath)
{
	ShaderProgramSource source(ParseShader(filepath));

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

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fargment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			else if (line.find("geometry") != std::string::npos)
				type = ShaderType::GEOMETRY;
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

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource, 
	const std::string& geometrySource)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs, fs, gs;
	vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
	fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	if (geometrySource != "")
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometrySource);
		GLCall(glAttachShader(program, gs));
	}

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	if (geometrySource != "")
		GLCall(glDeleteShader(gs));

	return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()););
	if (location == -1)
		std::cout << "Uniform doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
