#include "Mesh.h"

#include "ErrorCatch.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureMesh> textures)
{
	this->m_Vertices = vertices;
	this->m_Indices = indices;
	this->m_TextureMeshs = textures;

	SetUpMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_TextureMeshs.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		std::string number;
		std::string name = m_TextureMeshs[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetUniform1f(("material." + name + number).c_str(), i);
	}
}

void Mesh::SetUpMesh()
{
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(m_Vertices, m_Vertices.size() * sizeof(Vertex));
	m_IBO = std::make_unique<IndexBuffer>(m_Indices, m_Indices.size() * sizeof(unsigned int));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<unsigned int>(4);
	layout.Push<float>(4);
	m_VAO->AddBuffer(*m_VBO, layout);

	m_VAO->Unbind();
}
