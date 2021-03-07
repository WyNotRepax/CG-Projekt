#include <iostream>

// GLEW loads the function from the libraries provided with your Graphicscard driver
// defining GLEW_STATIC tells the glew that you want to link with the static version of glew (glew32s.lib instead of glew32.lib+glew32.dll)
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW is the window creation library
#include <GLFW/glfw3.h>
#include "shader/ConstantShader.h"
#include "math/Vector.h"
#include "renderer/GameRenderer.h"
#include "renderer/DebugRenderer.h"
#include "Logging.h"

#define _USE_MATH_DEFINES
#include <math.h>

GameRenderer* pGameRenderer;
Camera* pCamera;

static GLuint CompileShader(GLenum type, const std::string& source) {
	GLuint id = glCreateShader(type);
	const char* pSource = source.c_str();
	glShaderSource(id, 1, &pSource, nullptr);
	glCompileShader(id);

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		delete[] message;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLuint programID = glCreateProgram();
	GLuint vsID = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fsID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(programID, vsID);
	glAttachShader(programID, fsID);
	glLinkProgram(programID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return programID;
}

int main() {

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

	pCamera = new Camera(Matrix().lookAt(Vector(0, 0, 0), Vector(0, 1, 0), Vector(5, 2, 0)), Matrix().perspective(70, 16.0f / 9.0f, 0.01, 20));
	pGameRenderer = new GameRenderer(pCamera);

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};


	//GLuint bufferId;
	//glGenBuffers(1, &bufferId);
	//glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

	// Enable Attribute 0
	//glEnableVertexAttribArray(0);
	// Tell the GPU about your layout
	// index -> index of the Attribute
	// size -> number of components of the attribute
	// type -> type of each component
	// normalized -> sould the data be normalized
	// stride -> size of the whole vertex element in the buffer
	// pointer -> offset of the attribute within a vertex element (See "offsetof" macro?)
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//ConstantShader* pConstantShader = ConstantShader::GetInstance();
	//pConstantShader->setModelViewProj(Matrix().identity());
	Matrix testOrientation = Matrix().identity();
	Matrix testOrientationDelta = Matrix().roationAxis(M_PI * 2 / (8 * 60.0f),Vector(0,1,0).normalize());

	Model model = Model(MODEL_DIR"/bahn.dae");

	DebugRenderer::setCamera(pCamera);

	AABB aabb(Vector(-1, -1, -1), Vector(1, 1, 1));
	float n = 0;
	LOG("After\n");
	// Main Application loop
	int frameN = 0;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		pCamera->setView(pCamera->getView() * Matrix().rotationY(0.01f));
		//pConstantShader->setColor(n, 0.0f, 0.0f);
		if (frameN < 120 || true)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//pConstantShader->activate();
		//glDrawArrays(GL_LINE_STRIP, 0, 3);
		//testOrientation *= testOrientationDelta;
		DebugRenderer::drawAABB(aabb,testOrientation);
		//DebugRenderer::drawLine(Vector(0, 0, 0), Vector(0, 1, 0),testOrientation);
		//DebugRenderer::drawSphere(Vector(0,0,0),0.5f);
		model.draw(pCamera);
		glfwSwapBuffers(window);

		glfwPollEvents();
		GLenum Error = glGetError();
		if (Error != GL_NO_ERROR) {
			fprintf(stderr, "Error Code: %x\n", Error);
			//exit(1);
		}
		if (frameN == 60) {
			//testOrientationDelta *= Matrix().rotationZ(M_PI * 2 / (-2 * 60.0f));
		}
		frameN++;
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}