#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Renderer/Mesh.h>
#include <Renderer/Texture.h>

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
	Model() {}
	Model(const char* filepath);

	void Draw(Shader& shader);

private:

	void LoadModel(const char* filepath);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureRef> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

	std::vector<Mesh> m_meshes;
	std::string m_directory;

	std::unordered_map<std::string, TextureRef> m_textureCache;
};