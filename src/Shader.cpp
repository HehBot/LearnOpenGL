#include "OpenGL.hpp"

#include <string>
#include <fstream>
#include <iostream>

#include "Shader.hpp"
	
void Shader::ParseShader(std::string const &filepath, unsigned int type)
{
	std::ifstream file(filepath);
	std::string line;
	
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, BOTH = 2
	};
	
	if (type == 0){
		ShaderType type = ShaderType::NONE;

		std::string source[2];
		source[0] = source[1] = "";

		while(std::getline(file, line)){
			if (line.find("#shader") != std::string::npos){
				if (line.find("vertex") != std::string::npos){
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos){
					type = ShaderType::FRAGMENT;
				}
				else {
					type = ShaderType::NONE;
				}
			}
			else {
				source[(int)type] += (line + '\n');
			}
		}
		
		vertexShaderSource = source[0], fragmentShaderSource = source[1];
	}
	
	else {
		std::string source("");

		while (std::getline(file, line)){
			source += (line + '\n');
		}
		
		if (type == GL_VERTEX_SHADER)
			vertexShaderSource = source;
		else if (type == GL_FRAGMENT_SHADER)
			fragmentShaderSource = source;
	}
}

unsigned int Shader::CompileShader(unsigned int type, std::string const &source) const
{
	unsigned int id;
	
	// create shader
	id = glCreateShader(type);
	char const *src = source.c_str();
	
	// compile shader
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	
	// error detection
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if	(!success){
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED:\n" <<
		infoLog << "\n\n";
	}
	
	return id;
}

Shader::Shader(std::string const &filePath)
{
	// create shader program
	ID = glCreateProgram();
	
	// read shader source
	ParseShader(filePath);
	
	// compile shaders
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	// attach and link shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glValidateProgram(ID);
	
	// error detection
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if	(!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" <<
		infoLog << "\n\n";
	}
	
	// delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(std::string const &vsFilePath, std::string const &fsFilePath)
{
	// create shader program
	ID = glCreateProgram();
	
	// read shader source
	ParseShader(vsFilePath, GL_VERTEX_SHADER);
	ParseShader(fsFilePath, GL_FRAGMENT_SHADER);
	
	// compile shaders
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	// attach and link shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glValidateProgram(ID);
	
	// error detection
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if	(!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" <<
		infoLog << "\n\n";
	}
	
	// delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
	
void Shader::use() const
{
	glUseProgram(ID);
}

// vec1
void Shader::setInt(std::string const &name, int const &value1) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value1);
}
void Shader::setFloat(std::string const &name, float const &value1) const
{
	use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value1);
}

// vec2
void Shader::setVec2i(std::string const &name, int const &value1, int const &value2) const
{
	use();
	glUniform2i(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setVec2f(std::string const &name, float const &value1, float const &value2) const
{
	use();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setVec2f(std::string const &name, glm::vec2 const &vec2) const
{
	use();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), vec2.x, vec2.y);
}

// vec3
void Shader::setVec3i(std::string const &name, int const &value1, int const &value2, int const &value3) const
{
	use();
	glUniform3i(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setVec3f(std::string const &name, float const &value1, float const &value2, float const &value3) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setVec3f(std::string const &name, float const &value, glm::vec2 const &vec2) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value, vec2.x, vec2.y);
}
void Shader::setVec3f(std::string const &name, glm::vec2 const &vec2, float const &value) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec2.x, vec2.y, value);
}
void Shader::setVec3f(std::string const &name, glm::vec3 const &vec3) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
}

// vec4
void Shader::setVec4i(std::string const &name, int const &value1, int const &value2, int const &value3, int const &value4) const
{
	use();
	glUniform4i(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
void Shader::setVec4f(std::string const &name, float const &value1, float const &value2, float const &value3, float const &value4) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
void Shader::setVec4f(std::string const &name, float const &value1, float const &value2, glm::vec2 const &vec2) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, vec2.x, vec2.y);
}
void Shader::setVec4f(std::string const &name, float const &value1, glm::vec2 const &vec2, float const &value2) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, vec2.x, vec2.y, value2);
}
void Shader::setVec4f(std::string const &name, glm::vec2 const &vec2, float const &value1, float const &value2) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec2.x, vec2.y, value1, value2);
}
void Shader::setVec4f(std::string const &name, glm::vec2 const &vec21, glm::vec2 const &vec22) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec21.x, vec21.y, vec22.x, vec22.y);
}
void Shader::setVec4f(std::string const &name, float const &value1, glm::vec3 const &vec3) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, vec3.x, vec3.y, vec3.z);
}
void Shader::setVec4f(std::string const &name, glm::vec3 const &vec3, float const &value1) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z, value1);
}
void Shader::setVec4f(std::string const &name, glm::vec4 const &vec4) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
}

// mat[2/3/4]
void Shader::setMat2(std::string const &name, glm::mat2 const &matrix2f) const
{
	use();
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix2f));
}
void Shader::setMat3(std::string const &name, glm::mat3 const &matrix3f) const
{
	use();
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix3f));
}
void Shader::setMat4(std::string const &name, glm::mat4 const &matrix4f) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix4f));
}
