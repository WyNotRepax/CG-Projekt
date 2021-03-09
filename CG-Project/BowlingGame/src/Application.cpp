#include <iostream>

// GLEW loads the function from the libraries provided with your Graphicscard driver
// defining GLEW_STATIC tells the glew that you want to link with the static version of glew (glew32s.lib instead of glew32.lib+glew32.dll)
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW is the window creation library
#include <GLFW/glfw3.h>

#include"renderer/Camera.h"
#include "Logging.h"
#include "renderer/DebugRenderer.h"
#include "game/GameObject.h"
#include "game/BowlingBall.h"
#include "game/Pin.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera* pCamera = nullptr;
GLFWwindow* pWindow = nullptr;

std::vector<GameObject*> gameObjects;

void Draw();
void Update(float dt);
void Setup();
void SetupGL();
void CheckErrors();

int main() {



	SetupGL();
	Setup();
	// Main Application loop
	
	float lastTime = 0;
	while (!glfwWindowShouldClose(pWindow)) {
		glfwPollEvents();

		float currentTime = glfwGetTime();
		Update(currentTime - lastTime);
		lastTime = currentTime;

		Draw();

		glfwSwapBuffers(pWindow);
		CheckErrors();
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void SetupGL() {
	// Initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Create GLFW window
	pWindow = glfwCreateWindow(1280, 720, "Bowling Spiel", NULL, NULL);
	if (!pWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Create OpenGL Rendering Context and make that Context the current one
	glfwMakeContextCurrent(pWindow);


	// Initialize GLEW
	// IMPORTANT glewInit cannot be called without a valid OpenGL Rendering Context
	// -> after glfwMakeContextCurrent(...)
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Enable Depth testing
	LOG_CALL(glEnable, GL_DEPTH_TEST);
}

void Draw() {
	LOG_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
		(*it)->draw(pCamera);
	}
}

void Update(float dt) {
	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
		(*it)->update(dt);
	}
}

void Setup() {
	pCamera = new Camera(Matrix().lookAt(Vector(0, 0, 0), Vector(0, 1, 0), Vector(5, 2, 0)), Matrix().perspective(70, 16.0f / 9.0f, 0.01, 100));

	DebugRenderer::setCamera(pCamera);

	Light light;
	light.Position = Vector(0, 5, 0);
	light.Direction = Vector(1, -1, 0);
	light.Attenuation = Vector(2, 0, 0.01);
	light.SpotRadius = Vector(M_PI / 5, M_PI / 3, 0);
	light.Type = Light::DIRECTIONAL;
	GameShader::GetInstance()->addLight(light);
	light.Direction = Vector(-1, -1, 0);
	GameShader::GetInstance()->addLight(light);

	GameObject* pGameObject = new BowlingBall();
	pGameObject->setVelocity(Vector(1, 0, 0));
	//gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	gameObjects.emplace_back(pGameObject);
}

void CheckErrors() {
	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR) {
		fprintf(stderr, "Error Code: %x\n", Error);
		exit(1);
	}
}