#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <memory>

class Shader;
class Texture;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCroods;
};

class Mesh
{
public:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<std::shared_ptr<Texture>> m_Textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
	void Draw(Shader shader);
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_IBO;
	void SetUpMesh();
};