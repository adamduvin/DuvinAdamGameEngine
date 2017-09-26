#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <string>

struct Vertex {
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd {
	GLuint locInd;
	GLuint uvInd;
	GLuint normInd;
};

class Model
{
private:
	GLuint vertArr;
	GLuint vertBuf;
	GLuint vertCount;
public:
	Model();
	virtual ~Model();
	
	std::string objFilename;
	glm::vec3 maxXYZ;
	glm::vec3 minXYZ;

	bool buffer(std::string objFile);
	void render();
	void remove();
};

