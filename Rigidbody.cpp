#include "Rigidbody.h"
#include "Engine.h"


Rigidbody::Rigidbody()
{
}

// Initalizes mass, whether or not object has gravity, and the drag constant value
Rigidbody::Rigidbody(GLfloat mass, GLboolean hasGravity, GLfloat dragConst)
{
	this->mass = mass;
	this->hasGravity = hasGravity;
	this->dragConst = dragConst;
}


Rigidbody::~Rigidbody()
{
}

// Calculates and returns the object's velocity using forces
glm::vec3 Rigidbody::calcVelocity(GLFWwindow * window, glm::vec3 appliedForce)
{
	glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);

	if (hasGravity) {
		force += glm::vec3(0.0f, -9.8f, 0.0f) * mass;
	}

	drag = velocity * dragConst;

	force += drag;

	force += appliedForce;

	velocity += (force * Engine::deltaTime) / mass;

	return velocity;
}
