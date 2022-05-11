#include "OpenGL.hpp"

#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>

unsigned int Texture::n = 0;

Texture::Texture(std::string const &filepath, unsigned int wrapping):
	path(filepath)
{
	glGenTextures(1, &ID);
	// activate texture unit
	texture_unit = n;
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, ID);
	// set up the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	
	unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	
	std::string extn(filepath.substr(filepath.size() - 3));
	
	if (data != nullptr){
		// TODO
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (extn == "png" ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\tFailed to load texture " << filepath << '\n';
	}
	stbi_image_free(data);
	n += 1;
}

Texture::Texture(std::string const &filepath, GLint internal_format, GLenum format, unsigned int wrapping):
	path(filepath)
{
	glGenTextures(1, &ID);
	// activate texture unit
	texture_unit = n;
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, ID);
	// set up the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	
	unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	
	std::string extn(filepath.substr(filepath.size() - 3));
	
	if (data != nullptr){
		// TODO
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);
	n += 1;
}

void Texture::setUniform(Shader const &shader, std::string const &uniform_name) const
{
	shader.setInt(uniform_name.c_str(), texture_unit);
}

void Texture::bind(Shader const &shader) const
{
	shader.use();
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}
