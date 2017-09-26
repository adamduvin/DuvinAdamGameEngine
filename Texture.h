#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <string>

class Texture
{
private:
	GLuint texID;
	std::string texFilename;
public:
	Texture();
	~Texture();

	void init(std::string name);
	void apply();
	void remove();
};

