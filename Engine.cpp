#include "Engine.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include <vector>
#include <time.h>

//stuff
GLfloat Engine::prevTime;
GLfloat Engine::curTime;
GLfloat Engine::deltaTime;

glm::vec3 Engine::light;
ShaderProgram Engine::shaders;
GLFWwindow* Engine::GLFWwindowPtr;

Camera Engine::mainCamera;
GLint Engine::w;
GLint Engine::h;

std::vector<Object> Engine::objects;

bool Engine::shouldClose;

Engine::Engine()
{
}


Engine::~Engine()
{
}

bool Engine::init()
{
	// Checks if GLFW initialized properly
	if (glfwInit() == GL_FALSE) {
		return false;
	}

	// Creates window as a pointer
	GLFWwindowPtr = glfwCreateWindow(800, 600, "Adam Duvin DSA1 Engine", NULL, NULL);

	// Terminates if null
	if (GLFWwindowPtr != nullptr) {
		glfwMakeContextCurrent(GLFWwindowPtr);
	}

	else {
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	srand((unsigned int)time(0));	// RNG

	// Background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Performs setup
	loadObjects();
	loadShaders();
	initCamera();
	initLight();
	
	shouldClose = false;

	return true;
}

// Loads all models in the scene
void Engine::loadObjects()
{
	

	// Begin Model Loading

	// Load Player
	Object player;
	player.init("models/PlayerShip.obj", "textures/PlayerShipDiffuse.png", glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, true, 1.0f, false, -5.0f, Object::Sphere, 50.0f, false, false);
	objects.push_back(player);

	// Load Asteroids
	for (int i = 0; i < 10; i++) {
		Object asteroid;
		GLfloat signX, signY, signZ;

		// Random location
		if (rand() % 2 == 0) {
			signX = 1.0f;
		}
		else signX = -1.0f;

		if (rand() % 2 == 0) {
			signY = 1.0f;
		}
		else signY = -1.0f;

		if (rand() % 2 == 0) {
			signZ = 1.0f;
		}
		else signZ = -1.0f;

		asteroid.init("models/Asteroid.obj", "textures/AsteroidTexture.png", glm::vec3{ signX*(rand() % 30), signY*(rand() % 30), signZ*(rand() % 30) }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 3.0f, 3.0f, 3.0f }, false, 1.0f, false, -5.0f, Object::Sphere, 50.0f, false, true);		//Asteroid texture pattern: https://www.filterforge.com/filters/13302.jpg
		objects.push_back(asteroid);
	}
	
	// Load Targets
	for (int i = 0; i < 5; i++) {
		Object target;
		GLfloat signX, signY, signZ;

		// Random location
		if (rand() % 2 == 0) {
			signX = 1.0f;
		}
		else signX = -1.0f;

		if (rand() % 2 == 0) {
			signY = 1.0f;
		}
		else signY = -1.0f;

		if (rand() % 2 == 0) {
			signZ = 1.0f;
		}
		else signZ = -1.0f;

		target.init("models/Target.obj", "textures/TargetTexture.png", glm::vec3{ signX*(rand() % 30), signY*(rand() % 30), signZ*(rand() % 30) }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 2.0f, 2.0f, 2.0f }, false, 1.0f, true, -5.0f, Object::AABB, 50.0f, true, false);
		objects.push_back(target);
	}
	
}

// Loads shaders
void Engine::loadShaders()
{
	// Creates a shader program that loads the vertex and fragment shaders
	shaders.load();
	shaders.use();
}

// Initializes the main camera
void Engine::initCamera()
{
	mainCamera.initPlayer(objects[0]);

	// Declare the camera
	w = 800;
	h = 600;
	glfwGetWindowSize(GLFWwindowPtr, &w, &h);	// Gets the size of the window
	glfwSetCursorPos(GLFWwindowPtr, w / 2, h / 2);	// Sets the pointer to the center of the screen

	glEnable(GL_DEPTH_TEST);	// Enables depth test

	glEnable(GL_CULL_FACE);		// Enables backface culling
}

// Initializes the light
void Engine::initLight()
{
	light = glm::vec3(0.0f, 10.0f, 10.0f);	// Declare and initialize light as a hard-coded vec3

	glUniform3fv(20, 1, &light.x);	// Uploads light to the fragment shader as a uniform
}

void Engine::calcTime()
{
	prevTime = curTime;
	curTime = (GLfloat)glfwGetTime();
	deltaTime = curTime - prevTime;
}

// Runs the game scene
void Engine::gameLoop()
{
	while (!glfwWindowShouldClose(GLFWwindowPtr)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clears the frame

		for (int i = 0; i < objects.size(); i++) {
			objects[i].update(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f });
			
			for (int j = 0; j < objects.size(); j++) {
				if (i != j) {
					objects[i].collidesWith(objects[j]);	// Check collisions
				}
			}
			

			objects[i].render();		// Renders the object
			glBindVertexArray(0);	// Binds model's position array
		}
		

		glfwSwapBuffers(GLFWwindowPtr);		// Swaps the buffers

		calcTime();		// Calculates change in time (delta time)
		mainCamera.controlCamera(GLFWwindowPtr, deltaTime);	// Handels camera movement
		mainCamera.calcMatrix();	// Calculates camera matrix
		mainCamera.uploadMatrix();	// Uploads the matrix to the shaders

		if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_ESCAPE) || shouldClose) {
			break;	// End the game loop
		}

		glfwPollEvents();
	}
}

// Terminates GLFW
void Engine::terminate()
{
	for (int i = 0; i < 3; i++) {
		objects[i].remove();
	}

	glfwTerminate();	// Terminates GLFW
}
