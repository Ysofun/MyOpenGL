#pragma once

#include "Mesh.h"
#include <assimp/scene.h>
#include <unordered_map>

class Model
{
public:
	Model(const std::string& path);
	~Model();
	void Draw(const Shader& shader);
	std::vector<Mesh*> m_Meshes;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_TexturesCache;

private:
	
	std::string m_Directory;
	
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);
};