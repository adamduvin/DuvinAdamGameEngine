#include "Camera.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>


Camera::Camera()
{
	GLfloat zoom = 1.f;
	GLint width = 800;
	GLint height = 600;

	fov = 3.14159f * .4f / zoom;
	aspectRatio = (GLfloat)width / (GLfloat)height;
	nearPlaneDist = .01f;
	farPlaneDist = 1000.f;

	transform.translation = { 0, 1, 3.5f };
	transform.rotation = { 0, 0, 0 };

	rigidbody = Rigidbody(1.0f, false, -10.0f);
}

Camera::~Camera()
{
}

// Initializes a pointer to the player object
void Camera::initPlayer(Object& player)
{
	this->player = &player;
	transform = this->player->transform;
	transform.translation.z += 2.0f;
}

// Calculates camera/worldview matrix
void Camera::calcMatrix()
{
	// Perspective
	glm::mat4 perspectiveMat = glm::perspective(fov, aspectRatio, nearPlaneDist, farPlaneDist);

	// Look-At
	glm::mat3 rotationMatrix = (glm::mat3)glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);

	glm::vec3 eye = transform.translation;
	glm::vec3 center = eye + rotationMatrix * glm::vec3(0, 0, -1);
	glm::vec3 up = rotationMatrix * glm::vec3(0, 1, 0);

	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);

	worldView = perspectiveMat * lookAtMat;
}

// Uploads camera matrix to shaders
void Camera::uploadMatrix()
{
	glUniformMatrix4fv(4, 1, GL_FALSE, &worldView[0][0]);
	glUniform3fv(30, 1, &transform.translation.x);	// Upload camera location uniform to fragment shader
	
}

// Handles camera movement
void Camera::controlCamera(GLFWwindow* window, GLfloat deltaTime)
{
	glfwGetCursorPos(window, &x, &y);

	transform.rotation.y -= GLfloat(sens * (x - w * .5f));	// Yaw
	transform.rotation.x -= GLfloat(sens * (y - h * .5f));	// Pitch
	transform.rotation.x = glm::clamp(transform.rotation.x, -.5f * 3.14159f, .5f * 3.14159f);

	glfwSetCursorPos(window, w * .5f, h * .5f);		// Sets cursor to center of screen

	player->transform.rotation = transform.rotation;	// Camera follows the player

	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);

	glm::vec3 location;
	location = player->transform.translation;
	location += R * glm::vec3(0, 1, 3.5f);	// Resets camera behind the player

	transform.translation = location;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);		// Hides cursor
}
