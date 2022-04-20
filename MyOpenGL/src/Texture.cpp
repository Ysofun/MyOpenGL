#include "Texture.h"
#include "ErrorCatch.h"

#include <glad/glad.h>
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <iostream>

unsigned int num = 0;

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_FilePath(filepath), 
	m_Width(0), m_Height(0), m_BPP(0)
{
	//std::cout << filepath.c_str() << std::endl;
	//stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);
	//std::string str = "res/Writes/write" + std::to_string(num++) + ".png";
	//stbi_write_png(str.c_str(), m_Width, m_Height, m_BPP, m_LocalBuffer, m_Width * m_BPP);
	//std::cout << std::oct;
	//std::cout << (int)m_LocalBuffer << std::endl;
	GLCall(glGenTextures(1, &m_RendererID));

	if (m_LocalBuffer)
	{
		GLenum format;
		if (m_BPP == 1)
			format = GL_RED;
		else if (m_BPP == 3)
			format = GL_RGB;
		else if (m_BPP == 4)
			format = GL_RGBA;

		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
		glGenerateMipmap(GL_TEXTURE_2D);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(m_LocalBuffer);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
