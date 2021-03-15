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
#include "game/Background.h"
#include "game/Arrow.h"
#include "shader/GameShader.h"
#include "shader/ShadowShader.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum class Gamestate {
	selectPos,
	selectAng,
	selectPower,
	simulating
};


Camera* pCamera = nullptr;
GLFWwindow* pWindow = nullptr;
Gamestate state;
bool followCam;
std::vector<GameObject*> gameObjects;
BowlingBall* pBall;
Arrow* pArrow;

void Draw();
void Update(float dt);
void UpdateSimulation(float dt);
void UpdateSelectPos(float dt);
void UpdateSelectAng(float dt);
void UpdateSelectPower(float dt);
void Setup();
void SetupGL();
void CheckErrors();
void ResolveCollisions();
bool switchState();
bool switchCam();
bool SimulationDone();

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
	glEnable(GL_CULL_FACE);
}

void Draw() {
	GameShader::GetInstance()->generateShadows(gameObjects);

	LOG_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto pGameObject : gameObjects) {
		pGameObject->draw(pCamera, GameShader::GetInstance());
	}
}

void Update(float dt) {
	pArrow->setPosition(pBall->getPosition());
	switch (state) {
	case Gamestate::simulating:
		UpdateSimulation(dt);
		break;
	case Gamestate::selectPos:
		UpdateSelectPos(dt);
		break;
	case Gamestate::selectAng:
		UpdateSelectAng(dt);
		break;
	case Gamestate::selectPower:
		UpdateSelectPower(dt);
		break;
	}
	Vector pos; 
	if (switchCam()) {
		followCam = !followCam;
	}
	if (followCam) {
		pos = pBall->getPosition();
	}
	pCamera->setView(Matrix().lookAt(pos + Vector(0, 0, -9), Vector(0, 1, 0), pos + Vector(0, 1, 5)));
}

void UpdateSimulation(float dt) {
	for (auto pGameObject : gameObjects) {
		pGameObject->update(dt);
	}
	ResolveCollisions();

	if (SimulationDone()) {
		state = Gamestate::selectPos;
		pBall->setPosition(Vector(0, 0, 0));
	}
}

float selectedPosAng;
static const float speed = 1;
float maxPos = 0.5;
void UpdateSelectPos(float dt) {
	static const float speed = 1;
	selectedPosAng += dt * speed;
	selectedPosAng = fmod(selectedPosAng, 2 * M_PI);
	pBall->setPosition(Vector(cos(selectedPosAng) * maxPos, 0, 0));
	if (switchState()) {
		state = Gamestate::selectAng;
		pArrow->activate();
	}
}

float selectedAngAng = 0;
float selectedAng = 0;
float maxAng = M_PI / 8;
void UpdateSelectAng(float dt) {
	static const float speed = 1;
	selectedAngAng += dt * speed;
	selectedAng = cos(selectedAngAng) * maxAng;
	pArrow->setRotation(selectedAng);
	if (switchState()) {
		state = Gamestate::selectPower;
	}
}
float selectedPower = 0;
float powerSelectSpeed = 1;
float maxPower = 5;
float minPower = 1;

void UpdateSelectPower(float dt) {
	selectedPower = fmod(selectedPower + powerSelectSpeed * dt, 2);
	float calculatedPower = minPower + (maxPower - minPower) * (selectedPower - 1) * (selectedPower - 1);
	pArrow->setPowerScale(calculatedPower);
	if (switchState()) {
		pBall->setVelocity(Vector(sin(selectedAng), 0, -cos(selectedAng)) * calculatedPower);
		pArrow->deactivate();
		state = Gamestate::simulating;
	}
}

void Setup() {
	pCamera = new Camera(Matrix().lookAt(Vector(0, 0, -18), Vector(0, 1, 0), Vector(0, 2, 5)), Matrix().perspective(0.4, 16.0f / 9.0f, 0.01, 100));

	DebugRenderer::setCamera(pCamera);

	Light light;
	light.Position = Vector(0, 5, 0);
	light.Direction = Vector(0, -1, -1);
	light.Attenuation = Vector(2, 0, 0.01);
	light.SpotRadius = Vector(M_PI / 7, M_PI / 3, 0);
	light.Color = Vector(0.1, 0.1, 0.1);
	light.Type = Light::DIRECTIONAL;
	//GameShader::GetInstance()->addLight(light);
	light.Direction = Vector(-1, -1, 0);
	GameShader::GetInstance()->addLight(light);
	light.Color = Vector(1, 1, 1);
	light.Direction = Vector(0, -1, 0);
	light.Position = Vector(0, 4, -5);
	light.Type = Light::SPOT;
	GameShader::GetInstance()->addLight(light);
	light.Position = Vector(0, 4, -10);
	GameShader::GetInstance()->addLight(light);
	light.Position = Vector(0, 4, -15);
	GameShader::GetInstance()->addLight(light);
	light.Position = Vector(0, 4, 0);
	GameShader::GetInstance()->addLight(light);
	light.Direction = Vector(0, -1, -1);
	light.Position = Vector(0, 4, -15);
	GameShader::GetInstance()->addLight(light);

	// Setup Ball
	pBall = new BowlingBall();
	pBall->setVelocity(Vector(0, 0, -5));
	pBall->setPosition(Vector(0.1, 0, 0));
	gameObjects.emplace_back(pBall);


	GameObject* pGameObject = new Pin();
	pGameObject->setPosition(Vector(0, 0, -18.395));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(0, 0, -18.895));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(-0.155, 0, -18.645));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(-0.155, 0, -19.145));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(-0.31, 0, -18.895));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(-0.465, 0, -19.145));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(0.155, 0, -18.645));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(0.155, 0, -19.145));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(0.31, 0, -18.895));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Pin();
	pGameObject->setPosition(Vector(0.465, 0, -19.145));
	gameObjects.emplace_back(pGameObject);

	pGameObject = new Background();
	gameObjects.emplace_back(pGameObject);

	pArrow = new Arrow();
	gameObjects.emplace_back(pArrow);

	state = Gamestate::selectPos;
}

void CheckErrors() {
	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR) {
		fprintf(stderr, "Error Code: %x\n", Error);
		exit(1);
	}
}

struct Collision {
	GameObject* pGameObject1;
	GameObject* pGameObject2;
	Collision(GameObject* p1, GameObject* p2) {
		pGameObject1 = p1;
		pGameObject2 = p2;
	}
};

void ResolveCollisions() {
	std::vector<struct Collision> collisions;
	for (int i1 = 0; i1 < gameObjects.size(); i1++) {
		for (int i2 = i1 + 1; i2 < gameObjects.size(); i2++) {
			GameObject* pGameObject1 = gameObjects[i1];
			GameObject* pGameObject2 = gameObjects[i2];
			if (pGameObject1->collidesWith(pGameObject2)) {
				collisions.emplace_back(pGameObject1, pGameObject2);
			}
		}
	}
	bool resolved = false;
	while (!resolved) {
		resolved = true;
		for (Collision& c : collisions) {
			if (c.pGameObject1->collideWith(c.pGameObject2)) {
				resolved = false;
			}
		}
	}
}

bool switchState() {
	static bool block = false;
	int state = glfwGetKey(pWindow, GLFW_KEY_SPACE);
	if (!block && state == GLFW_PRESS) {
		block = true;
		return true;
	}
	if (state == GLFW_RELEASE) {
		block = false;
	}
	return false;
}

bool switchCam() {
	static bool block = false;
	int state = glfwGetKey(pWindow, GLFW_KEY_C);
	if (!block && state == GLFW_PRESS) {
		block = true;
		return true;
	}
	if (state == GLFW_RELEASE) {
		block = false;
	}
	return false;
}

bool SimulationDone() {
	bool stillMoving = false;
	for (auto pGameObject : gameObjects) {
		if (!pGameObject->getActive()) {
			// Skip inaktive Objects
			continue;
		}
		if (pGameObject->getVelocity().magnitudeSquared() > 0) {
			stillMoving = true;
			break;
		}
	}
	return !stillMoving;
}