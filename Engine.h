#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Object.h"

#include <string>
#include <vector>

class Engine
{
public:
	Engine();
	~Engine();

	// Time
	static GLfloat prevTime;
	static GLfloat curTime;
	static GLfloat deltaTime;	// Change in time

	static glm::vec3 light;		// Main light
	static ShaderProgram shaders;	// Shader program
	static GLFWwindow* GLFWwindowPtr;	// Window

	// Camera
	static Camera mainCamera;	// Main scene camera (uses FPS controls)
	static GLint w;		// Camera width
	static GLint h;		// Camera height
	 
	// Collection of all objects in the scene
	static std::vector<Object> objects;

	static bool shouldClose;
	 
	// Engine's methods
	static bool init();		// Initializes the engine
	static void loadObjects();	// Loads all objects
	static void loadShaders();	// Loads shaders and shader program
	static void initCamera();	// Initializes the main camera
	static void initLight();	// Initializes the main light
	static void calcTime();		// Calculates change in time
	static void gameLoop();		// Runs the game until esc is pressed
	static void terminate();	// Terminates the engine and cleans up allocated memory
};

