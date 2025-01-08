#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h"
#include "../glm/gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string RED = "\033[1;31m";
const std::string WHITE = "\033[0m";

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve source code from file paths
		std::string vertexCodeStr, fragmentCodeStr;
		std::ifstream vertexFile, fragmentFile;
		vertexFile.exceptions	(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			std::stringstream vertexStream, fragmentStream;

			// read file's buffer content into streams
			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			// close file handlers
			vertexFile.close();
			fragmentFile.close();

			// convert stream into string
			vertexCodeStr = vertexStream.str();
			fragmentCodeStr = fragmentStream.str();
		} 
		catch (std::ifstream::failure e) {
			std::cout << RED << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << WHITE << std::endl;
		}

		const char* vertexShaderSource = vertexCodeStr.c_str();
		const char* fragmentShaderSource = fragmentCodeStr.c_str();

		// 2. compile and link shaders
		unsigned int vertex, fragment;
		int success;
		char log[512];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderSource, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, log);
			std::cout << RED << "ERROR::VERTEX::SHADER::COMPILATION_FAILED"
				<< log << WHITE << std::endl;
		}

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, log);
			std::cout << RED << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED"
				<< log << WHITE << std::endl;
		}

		// 3. shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, log);
			std::cout << RED << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				<< log << WHITE << std::endl;
		}

		// 4. clean up individual shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// activates shader program
	void use() const {
		glUseProgram(ID);
	}

	// queries uniform location and sets its value
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string &name, glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string &name, float s, float t, float u, float v) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), s, t, u, v);
	}
};

#endif
