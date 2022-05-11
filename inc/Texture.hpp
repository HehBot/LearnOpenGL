#pragma once

#include "Shader.hpp"

#include <string>

class Texture {
private:
	int width, height, nrChannels;
	int texture_unit;
public:
	unsigned int ID;
	static unsigned int n;
	std::string path;
	
	Texture(std::string const &filepath, unsigned int wrapping = GL_REPEAT);
	Texture(std::string const &filepath, GLint internal_format, GLenum format, unsigned int wrapping = GL_REPEAT);
	
	void setUniform(Shader const &shader, std::string const &uniform_name) const;
	void bind(Shader const &shader) const;
};
