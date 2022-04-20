#include "Mesh.h"

#include "ErrorCatch.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
	: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
	SetUpMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Bind(i);
		std::string number;
		std::string type = m_Textures[i]->GetTextureType();
		if (type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		//else if (type == "texture_specular")
		//	number = std::to_string(specularNr++);
		//else if (type == "texture_normal")
		//	number = std::to_string(normalNr++);
		//else if (type == "texture_height")
		//	number = std::to_string(heightNr++);

		shader.SetUniform1i("material." + type + "[" + number + "]", i);
	}
	shader.SetUniform1i("material.texture_diffuse_num", diffuseNr);
	//shader.SetUniform1i("material.texture_specular_num", specularNr);
	//shader.SetUniform1i("material.texture_normal_num", normalNr);
	//shader.SetUniform1i("material.texture_height_num", heightNr);

	m_VAO->Bind();
	m_IBO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));

	m_VAO->Unbind();
	m_IBO->Unbind();
	GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::SetUpMesh()
{
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(m_Vertices, m_Vertices.size() * sizeof(Vertex));
	m_IBO = std::make_unique<IndexBuffer>(m_Indices, m_Indices.size());

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VBO, layout);

	m_VAO->Unbind();
}
