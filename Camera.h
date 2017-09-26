#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Object.h"

class Camera
{
private:
	glm::mat4 worldView;
	GLfloat fov;
	GLfloat aspectRatio;
	GLfloat nearPlaneDist;
	GLfloat farPlaneDist;
	GLfloat sens = .005f;
	GLint w = 800, h = 600;
	GLdouble x, y;
	GLfloat speed = 1.f;
	
public:
	Camera();
	~Camera();

	Transform transform;
	Rigidbody rigidbody;

	Object* player;

	void initPlayer(Object& player);
	void calcMatrix();
	void uploadMatrix();
	void controlCamera(GLFWwindow* window, GLfloat deltaTime);
};

