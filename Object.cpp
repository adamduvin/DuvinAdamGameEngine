#include "Object.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Engine.h"
#include <cmath>

Object::Object()
{
}


Object::~Object()
{
}

// Initializes the game object
void Object::init(std::string objFile, std::string texFile, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, GLboolean controllable, GLfloat mass, GLboolean hasGravity, GLfloat dragConst, ColliderType collider, GLfloat maxVelocity, bool isTarget, bool isObstacle)
{
	model.buffer(objFile);	// Buffer model
	texture.init(texFile);	// Initialize texture
	transform.init(translation, rotation, scale, maxVelocity);	// Initialize transform

	this->controllable = controllable;	// Is this the player?
	rigidbody = Rigidbody(mass, hasGravity, dragConst);		// Sets up rigidbody
	this->collider = collider;	// Sets up collider
	this->isTarget = isTarget;	// Is this a target?
	this->isObstacle = isObstacle;	// Is this an obstacle
}

// Updates the object's model, texture, and transform
void Object::update(glm::vec3 nextTranslation, glm::vec3 nextRotation, glm::vec3 nextScale)
{
	// Move if it's the player
	if (controllable) {
		glm::vec3 location = transform.move(Engine::GLFWwindowPtr, rigidbody) * Engine::deltaTime;

		transform.update(location, nextRotation, nextScale);
	}

	// Otherwise stay still
	else {
		transform.update(nextTranslation, nextRotation, nextScale);
	}
}

// Checks collisions
bool Object::collidesWith(Object & other)
{
	// At least one does not have a collider
	if (collider == None || other.collider == None) {
		return false;
	}

	// Both have sphere colliders
	else if (collider == Sphere && other.collider == Sphere) {
		glm::vec3 distance = transform.translation - other.transform.translation;
		if (glm::dot(distance, distance) > pow(transform.scale.x + other.transform.scale.x, 2)) {
			return false;
		}
		
		// If the player is colliding with an obstacle, end the game
		if (controllable && other.isObstacle) {
			Engine::shouldClose = true;
		}

		return true;
	}

	// Both have AABB colliders
	else if (collider == AABB && other.collider == AABB) {
		GLfloat distanceX = transform.translation.x - other.transform.translation.x;
		distanceX = abs(distanceX);
		GLfloat distanceY = transform.translation.y - other.transform.translation.y;
		distanceY = abs(distanceY);
		GLfloat distanceZ = transform.translation.z - other.transform.translation.z;
		distanceZ = abs(distanceZ);

		GLfloat sumSizeX = transform.scale.x + other.transform.scale.x;
		GLfloat sumSizeY = transform.scale.y + other.transform.scale.y;
		GLfloat sumSizeZ = transform.scale.z + other.transform.scale.z;

		if (distanceX < sumSizeX) {
			if (distanceY < sumSizeY) {
				if (distanceZ < sumSizeZ) {
					return true;
				}
			}
		}

		return false;
	}

	// One has a sphere collider, the other has an AABB collider
	else if (collider == Sphere && other.collider == AABB) {
		Object* sphere;
		Object* box;
		if (collider == Sphere) {
			sphere = this;
			box = &other;
		}

		else {
			sphere = &other;
			box = this;
		}

		glm::vec3 distance = sphere->transform.translation - box->transform.translation;
		glm::vec3 e = box->transform.scale * box->model.maxXYZ;
		glm::vec3 nearestPoint = box->transform.translation + glm::clamp(distance, -e, e);
		distance = sphere->transform.translation - nearestPoint;

		if (glm::dot(distance, distance) > pow(sphere->transform.scale.x, 2)) {
			return false;
		}

		// If the player is colliding with a target, make the target disappear (player collects the supplies crate)
		if (controllable && other.isTarget) {
			other.remove();
		}
		return true;
	}

	return false;
}

// Renders the object
void Object::render()
{
	transform.upload();
	texture.apply();
	model.render();
}

// Deletes the object
void Object::remove()
{
	model.remove();
	texture.remove();
}
