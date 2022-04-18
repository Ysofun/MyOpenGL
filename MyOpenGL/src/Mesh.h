#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <memory>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

constexpr unsigned int MAX_BONE_INFLUECE = 4;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCroods;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	
	unsigned int m_BoneIDs[MAX_BONE_INFLUECE];
	float m_Weights[MAX_BONE_INFLUECE];
};

struct TextureMesh
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<TextureMesh> m_TextureMeshs;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureMesh> textureMeshs);
	void Draw(Shader shader);
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_IBO;
	void SetUpMesh();
};