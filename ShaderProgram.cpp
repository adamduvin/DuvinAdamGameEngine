#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include "ShaderProgram.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>



ShaderProgram::ShaderProgram()
{
}


ShaderProgram::~ShaderProgram()
{
}

// Loads vertex and fragment shaders
bool ShaderProgram::load()
{
	bool vert = compile(GL_VERTEX_SHADER);
	bool frag = compile(GL_FRAGMENT_SHADER);

	if (vert == false || frag == false) return false;

	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (success == 0) {
		GLint errorLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLength);

		GLchar* error = new GLchar[errorLength + 1];
		error[errorLength] = '\0';

		glGetProgramInfoLog(programID, errorLength, NULL, error);

		delete[] error;

		return false;
	}

	return true;
}

// Compiles and sets shaders to string variables
bool ShaderProgram::compile(GLenum shaderType)
{
	// Checks for shader type and sets an unsigned int to the shader's ID
	GLuint* shaderID = 0;
	if (shaderType == GL_VERTEX_SHADER) {
		shaderID = &vertShaderID;
	}

	else if (shaderType == GL_FRAGMENT_SHADER) {
		shaderID = &fragShaderID;
	}

	// Finds the name of the file and saves it as a string
	std::string fileName;
	if (shaderType == GL_VERTEX_SHADER) {
		fileName = vertexShader;
	}

	else if (shaderType == GL_FRAGMENT_SHADER) {
		fileName = fragShader;
	}

	// Reads the file
	std::ifstream instream(fileName, std::ios::binary);
	if (!instream.is_open()) {
		printf("Error occurred when opening shader\n");
		return false;
	}

	instream.seekg(0, std::ios::end);
	int length = (int)instream.tellg();
	instream.seekg(0, std::ios::beg);

	char* fileText = new char[length + 1];
	fileText[length] = '\0';
	instream.read(fileText, length);	// Saves file content in a char array

	instream.close();

	*shaderID = glCreateShader(shaderType);		// Sets ID to appropriate shader type
	glShaderSource(*shaderID, 1, &fileText, 0);		// Links shader ID and file content
	glCompileShader(*shaderID);		// Compiles the shader

	delete[] fileText;

	GLint success;
	glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);

	// Checks to see if compilation succeeded, throws error message if it did not
	if (success == 0) {
		GLint errorLength;
		glGetShaderiv(*shaderID, GL_INFO_LOG_LENGTH, &errorLength);

		GLchar* error = new GLchar[errorLength + 1];
		error[errorLength] = '\0';

		glGetShaderInfoLog(*shaderID, errorLength, 0, error);

		printf("Vertex Shader Error: %s\n", error);
		delete[] error;
		glDeleteShader(*shaderID);

		return false;
	}

	return true;
}

// Use the program
void ShaderProgram::use()
{
	glUseProgram(programID);
}

// Delete the program
void ShaderProgram::unload()
{
	glDeleteProgram(programID);
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	programID = 0;
	vertShaderID = 0;
	fragShaderID = 0;
}
