#include <iostream>

// GLEW loads the function from the libraries provided with your Graphicscard driver
// defining GLEW_STATIC tells the glew that you want to link with the static version of glew (glew32s.lib instead of glew32.lib+glew32.dll)
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW is the window creation library
#include <GLFW/glfw3.h>
#include "shader/ConstantShader.h"
#include "math/Vector.h"
#include "renderer/DebugRenderer.h"
#include "Logging.h"
#include "collider/SphereCollider.h"
#include "renderer/Model.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera* pCamera;

int main() {
	std::cout << "WTF";
	// Initialize GLFW
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Bowling Spiel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Create OpenGL Rendering Context and make that Context the current one
	glfwMakeContextCurrent(window);


	// Initialize GLEW
	// IMPORTANT glewInit cannot be called without a valid OpenGL Rendering Context
	// -> after glfwMakeContextCurrent(...)
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	// Print Current OpenGL Version
	std::cout << glGetString(GL_VERSION) << std::endl;
#endif

	pCamera = new Camera(Matrix().lookAt(Vector(0, 0, 0), Vector(0, 1, 0), Vector(5, 2, 0)), Matrix().perspective(70, 16.0f / 9.0f, 0.01, 100));

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};




	Matrix testOrientation = Matrix().identity();
	Matrix testOrientationDelta = Matrix().roationAxis(M_PI * 2 / (8 * 60.0f), Vector(0, 1, 0).normalize());

	Model model = Model(MODEL_DIR"/bahn.dae");
	SphereCollider sphereCollider = SphereCollider();

	Matrix cylinderTransform = Matrix().scale(0.5, 1.5, 0.5);

	DebugRenderer::setCamera(pCamera);



	AABB aabb(Vector(-1, -1, -1), Vector(1, 1, 1));
	float n = 0;
	LOG("After\n");
	// Main Application loop
	int frameN = 0;
	LOG_CALL(glEnable, GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		pCamera->setView(pCamera->getView() * Matrix().rotationY(0.01f));
		if (frameN < 120 || true)
		{
			LOG_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//pConstantShader->activate();
		//glDrawArrays(GL_LINE_STRIP, 0, 3);
		//testOrientation *= testOrientationDelta;
		//DebugRenderer::drawAABB(aabb,testOrientation);
		//DebugRenderer::drawLine(Vector(0, 0, 0), Vector(0, 1, 0),testOrientation);
		//DebugRenderer::drawSphere(Vector(0,0,0),0.5f);
		model.draw(pCamera);
		DebugRenderer::drawCylinder(cylinderTransform);
		glfwSwapBuffers(window);

		glfwPollEvents();
		GLenum Error = glGetError();
		if (Error != GL_NO_ERROR) {
			fprintf(stderr, "Error Code: %x\n", Error);
			//exit(1);
		}
		frameN++;
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}
