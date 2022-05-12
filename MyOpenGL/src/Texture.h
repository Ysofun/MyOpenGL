#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_TextureType;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	bool m_GammaCorrection;
public:
	Texture(const std::string& filepath, bool gammaCorrection);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline std::string GetFilePath() const { return m_FilePath; }
	inline std::string GetTextureType() const { return m_TextureType; }
	inline unsigned int GetRendererID() const { return m_RendererID; }

	inline void SetTextureType(const std::string& type) { m_TextureType = type; }
};