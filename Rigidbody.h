#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

class Rigidbody
{
private:
	glm::vec3 velocity;
	glm::vec3 drag;
	GLfloat dragConst;
	GLfloat mass;
	GLboolean hasGravity;
public:
	Rigidbody();
	Rigidbody(GLfloat mass, GLboolean hasGravity, GLfloat dragConst);
	~Rigidbody();

	glm::vec3 calcVelocity(GLFWwindow* window, glm::vec3 appliedForce);
};

