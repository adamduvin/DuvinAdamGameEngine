#include "Transform.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include "Rigidbody.h"

Transform::Transform()
{
}


Transform::~Transform()
{
}

// Initialize transform
void Transform::init(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, GLfloat maxVelocity)
{
	this->translation = translation;
	this->rotation = rotation;
	this->scale = scale;
	this->maxVelocity = maxVelocity;
}

// Move the player
glm::vec3 Transform::move(GLFWwindow * window, Rigidbody& rb)
{
	bool moving = false;

	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

	glm::vec3 appliedForce;

	// Move left
	if (glfwGetKey(window, GLFW_KEY_A)) {
		appliedForce += R * glm::vec3(-1, 0, 0);
		moving = true;
	}

	// Move right
	if (glfwGetKey(window, GLFW_KEY_D)) {
		appliedForce += R * glm::vec3(1, 0, 0);
		moving = true;
	}

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W)) {
		appliedForce += R * glm::vec3(0, 0, -1);
		moving = true;
	}

	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S)) {
		appliedForce += R * glm::vec3(0, 0, 1);
		moving = true;
	}

	// Stop moving
	if (!moving) {
		appliedForce = glm::vec3(0, 0, 0);
	}

	if (appliedForce != glm::vec3()) {
		appliedForce = glm::normalize(appliedForce) * 500.0f;
	}

	glm::vec3 velocity = rb.calcVelocity(window, appliedForce);

	if (dot(velocity, velocity) > pow(maxVelocity, 2)) {
		velocity = glm::normalize(velocity) * maxVelocity;
	}

	return velocity;
}

// Update transform
void Transform::update(glm::vec3 nextTranslation, glm::vec3 nextRotation, glm::vec3 nextScale)
{
	translation += nextTranslation;
	rotation += nextRotation;
	scale += nextScale;

	t = glm::translate(translation);
	r = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	s = glm::scale(scale);

	objectWorld = t * r * s;
}

// Upload transform to shader
void Transform::upload()
{
	glUniformMatrix4fv(5, 1, GL_FALSE, &objectWorld[0][0]);
}
