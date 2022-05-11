#include "Model.hpp"

Mesh::Mesh(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, std::vector<Texture> const &textures, std::vector<TextureType> const &types):
	vertices(vertices), indices(indices), textures(textures), types(types)
{
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) const
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int emissiveNr = 0;
	for (unsigned int i = 0; i < textures.size(); i++){
		std::string uniform_name("material.");
		switch (types[i]){
			case TextureType::DIFFUSE :
			{
				uniform_name += ("texture_diffuse" + std::to_string(diffuseNr++) + "");
				break;
			}
			case TextureType::SPECULAR :
			{
				uniform_name += ("texture_specular" + std::to_string(specularNr++) + "");
				break;
			}
			case TextureType::EMISSIVE :
			{
				uniform_name += ("texture_emissive" + std::to_string(emissiveNr++) + "");
				break;
			}
		}
		
		textures[i].bind(shader);
		textures[i].setUniform(shader, uniform_name);
	}
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::loadModel(std::string const &path)
{
	Assimp::Importer importer;
	aiScene const *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, aiScene const *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, aiScene const *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<TextureType> types; 
	
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		
		if (mesh->mTextureCoords[0]){
			vertex.TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);
		}
		
		vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}
	
	if (mesh->mMaterialIndex >= 0){
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<TextureType> temp;
		
		std::vector<Texture> diffuseMaps(loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE));
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		types = std::vector<TextureType>(diffuseMaps.size(), TextureType::DIFFUSE);
		
		std::vector<Texture> specularMaps(loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR));
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		temp = std::vector<TextureType>(specularMaps.size(), TextureType::SPECULAR);
		types.insert(types.end(), temp.begin(), temp.end());
		
		std::vector<Texture> emissiveMaps(loadMaterialTextures(material, aiTextureType_EMISSIVE, TextureType::EMISSIVE));
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
		temp = std::vector<TextureType>(emissiveMaps.size(), TextureType::EMISSIVE);
		types.insert(types.end(), temp.begin(), temp.end());
	}
	
	return Mesh(vertices, indices, textures, types);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if (directory[directory.size() - 1] != '/')
			directory += "/";
		std::string path = directory + std::string(str.C_Str());
		
		bool skip = false;
		for (auto t : textures_loaded){
			if (t.path == path){
				textures.push_back(t);
				skip = true;
				break;
			}
		}		
		
		if (!skip){
			std::cout << "Loading " << path << '\n';
			Texture texture(path.c_str());
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

Model::Model(std::string const &path)
{
	loadModel(path);
}

void Model::Draw(Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i].Draw(shader);
	}
}
