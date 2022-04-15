#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
	ShaderProgramSource(const std::string& vertexSource, const std::string& fragmentSource,
		const std::string& geometrySource = "")
		: VertexSource(vertexSource), FragmentSource(fragmentSource), GeometrySource(geometrySource)
	{
	}

	ShaderProgramSource(const ShaderProgramSource&& source) noexcept
		: VertexSource(source.VertexSource), FragmentSource(source.FragmentSource), 
		GeometrySource(source.GeometrySource)
	{
	}
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set uniforms
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
	int GetUniformLocation(const std::string& name) const;
};

#endif // !SHADER_H
