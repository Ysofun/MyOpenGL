#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class Model
{
public:
	Model(const std::string& path);
	void Draw(Shader shader);
private:
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	std::unordered_map<std::string, TextureMesh> m_TexturesCache;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureMesh> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);
};