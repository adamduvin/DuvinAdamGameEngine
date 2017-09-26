#pragma once

#include "Model.h"
#include "Texture.h"
#include "Transform.h"
#include "Rigidbody.h"

class Object
{
public:
	Object();
	~Object();

	enum ColliderType {
		Sphere,
		AABB,
		OBB,
		None
	};

	ColliderType collider;

	Model model;
	Texture texture;
	Transform transform;
	Rigidbody rigidbody;

	GLboolean controllable;

	bool isTarget;
	bool isObstacle;

	void init(std::string objFile, std::string texFile, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, GLboolean controllable, GLfloat mass, GLboolean hasGravity, GLfloat dragConst, ColliderType collider, GLfloat maxVelcoity, bool isTarget, bool isObstacle);
	void update(glm::vec3 nextTranslation, glm::vec3 nextRotation, glm::vec3 nextScale);
	bool collidesWith(Object& other);
	void render();
	void remove();
};

