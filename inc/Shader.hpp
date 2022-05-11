#pragma once

#include "OpenGL.hpp"

#include <string>
#include <fstream>
#include <iostream>

class Shader {
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	
	void ParseShader(std::string const &filepath, unsigned int type = 0);
	
	unsigned int CompileShader(unsigned int type, std::string const &source) const;
	
public:
	// the program ID
	unsigned int ID;
	
	// constructor reads and builds the shader
	Shader(std::string const &filePath);
	Shader(std::string const &vsFilePath, std::string const &fsFilePath);
	
	// use/activate the shader
	void use() const;
	
	// utility uniform functions
	// vec1
	void setInt(std::string const &name, int const &value) const;
	void setFloat(std::string const &name, float const &value) const;

	// vec2
	void setVec2i(std::string const &name, int const &value1, int const &value2) const;
	void setVec2f(std::string const &name, float const &value1, float const &value2) const;
	void setVec2f(std::string const &name, glm::vec2 const &vec2) const;

	// vec3
	void setVec3i(std::string const &name, int const &value1, int const &value2, int const &value3) const;
	void setVec3f(std::string const &name, float const &value1, float const &value2, float const &value3) const;
	void setVec3f(std::string const &name, float const &value1, glm::vec2 const &vec2) const;
	void setVec3f(std::string const &name, glm::vec2 const &vec2, float const &value1) const;
	void setVec3f(std::string const &name, glm::vec3 const &vec3) const;

	// vec4
	void setVec4i(std::string const &name, int const &value1, int const &value2, int const &value3, int const &value4) const;
	void setVec4f(std::string const &name, float const &value1, float const &value2, float const &value3, float const &value4) const;
	void setVec4f(std::string const &name, float const &value1, float const &value2, glm::vec2 const &vec2) const;
	void setVec4f(std::string const &name, float const &value1, glm::vec2 const &vec2, float const &value2) const;
	void setVec4f(std::string const &name, glm::vec2 const &vec2, float const &value1, float const &value2) const;
	void setVec4f(std::string const &name, glm::vec2 const &vec21, glm::vec2 const &vec22) const;
	void setVec4f(std::string const &name, float const &value1, glm::vec3 const &vec3) const;
	void setVec4f(std::string const &name, glm::vec3 const &vec3, float const &value1) const;
	void setVec4f(std::string const &name, glm::vec4 const &vec4) const;

	// mat[2/3/4]
	void setMat2(std::string const &name, glm::mat2 const &matrix2f) const;
	void setMat3(std::string const &name, glm::mat3 const &matrix3f) const;
	void setMat4(std::string const &name, glm::mat4 const &matrix4f) const;
};
