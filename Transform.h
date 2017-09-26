#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include "Rigidbody.h"

class Transform
{
public:
	Transform();
	~Transform();

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 objectWorld;

	glm::mat4 t;
	glm::mat4 r;
	glm::mat4 s;

	GLfloat maxVelocity;	// Cannot exceed this velocity

	void init(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, GLfloat maxVelocity);
	glm::vec3 move(GLFWwindow * window, Rigidbody& rb);
	void update(glm::vec3 nextTranslation, glm::vec3 nextRotation, glm::vec3 nextScale);
	void upload();
};

