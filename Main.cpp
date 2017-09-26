#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "Engine.h"

int main() {
	Engine::init();		// Initializes engine members
	
	Engine::gameLoop();		// Runs the game
	
	Engine::terminate();	// Cleans up everything GLFW related

	return 0;
}