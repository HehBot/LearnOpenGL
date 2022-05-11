#pragma once

#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

enum class TextureType {
	DIFFUSE = 0, SPECULAR = 1, EMISSIVE = 2
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;	
	void setupMesh();
public:
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<TextureType> types;
	
	Mesh(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, std::vector<Texture> const &textures, std::vector<TextureType> const &types);
	void Draw(Shader &shader) const;
};

class Model {
private:
	std::vector<Mesh> meshes;
	std::string directory;
	
	std::vector<Texture> textures_loaded;
	
	void loadModel(std::string const &path);
	void processNode(aiNode *node, aiScene const *scene);
	Mesh processMesh(aiMesh *mesh, aiScene const *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName);
public:
	Model(std::string const &path);
	void Draw(Shader &shader);
};
