#pragma once
#include <string>
class ShaderProgram
{
private:
	std::string vertexShader = "shaders/vShader.glsl";
	std::string fragShader = "shaders/fShader.glsl";
	GLuint programID;
	GLuint vertShaderID;
	GLuint fragShaderID;
public:
	ShaderProgram();
	~ShaderProgram();

	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
};

