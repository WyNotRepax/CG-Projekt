#include "DebugRenderer.h"

#include <GL/glew.h>

#include "../Logging.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define SPHERE_H_SEGMENT_COUNT 20
#define SPHERE_V_SEGMENT_COUNT 10
#define SPHERE_TOTAL_SEGMENT_COUNT SPHERE_H_SEGMENT_COUNT*SPHERE_V_SEGMENT_COUNT
#define CYLINDER_SEGMENT_COUNT 20

bool DebugRenderer::sInit = true;
Camera* DebugRenderer::pCamera = nullptr;
bool DebugRenderer::sDestroyCamera = false;
ConstantShader* DebugRenderer::pShader = nullptr;

GLuint DebugRenderer::sLineVertexBufferId = 0;
GLuint DebugRenderer::sLineVaoId = 0;

GLuint DebugRenderer::sAABBVertexBufferId = 0;
GLuint DebugRenderer::sAABBVaoId = 0;
GLuint DebugRenderer::sAABBIndexBufferId = 0;

GLuint DebugRenderer::sSphereVertexBufferId = 0;
GLuint DebugRenderer::sSphereVaoId = 0;
GLuint DebugRenderer::sSphereIndexBufferId = 0;
unsigned int DebugRenderer::sSphereIndexCount = 0;


GLuint DebugRenderer::sCylinderVaoId = 0;
GLuint DebugRenderer::sCylinderIndexBufferId = 0;
unsigned int DebugRenderer::sCylinderIndexCount = 0;

void DebugRenderer::init()
{
	if (pCamera == nullptr) {
		pCamera = new Camera();
		sDestroyCamera = true;
	}
	pShader = ConstantShader::GetInstance();

	//Generate stuff for Line drawing
	LOG_CALL(glGenVertexArrays, 1, &sLineVaoId);
	LOG_CALL(glBindVertexArray, sLineVaoId);

	LOG_CALL(glGenBuffers, 1, &sLineVertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, sLineVertexBufferId);
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 2 * 3, NULL, GL_DYNAMIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	LOG_CALL(glBindVertexArray, 0);

	// Genereate stuff for AABB drawing
	LOG_CALL(glGenVertexArrays, 1, &sAABBVaoId);
	LOG_CALL(glBindVertexArray, sAABBVaoId);

	LOG_CALL(glGenBuffers, 1, &sAABBVertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, sAABBVertexBufferId);

	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, NULL, GL_DYNAMIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	LOG_CALL(glGenBuffers, 1, &sAABBIndexBufferId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sAABBIndexBufferId);
	static const unsigned short indeces[] = {
		0, 1,
		0, 2,
		0, 4,
		1, 3,
		1, 5,
		2, 3,
		2, 6,
		3, 7,
		4, 6,
		4, 5,
		5, 7,
		6, 7
	};
	LOG_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	//Setup stuff for sphere drawing

	LOG_CALL(glGenVertexArrays, 1, &sSphereVaoId);
	LOG_CALL(glBindVertexArray, sSphereVaoId);

	LOG_CALL(glGenBuffers, 1, &sSphereVertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, sSphereVertexBufferId);

	LOG_CALL(glGenBuffers, 1, &sSphereIndexBufferId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sSphereIndexBufferId);

	float sphereVertexData[SPHERE_TOTAL_SEGMENT_COUNT * 3];
	std::vector<unsigned short> sphereIndexData;
	for (unsigned int yIndex = 0; yIndex < SPHERE_V_SEGMENT_COUNT; yIndex++) {
		float vAngle = M_PI * ((float)yIndex / (float)(SPHERE_V_SEGMENT_COUNT - 1));
		float r = sin(vAngle);
		float y = cos(vAngle);
		for (unsigned int xIndex = 0; xIndex < SPHERE_H_SEGMENT_COUNT; xIndex++) {
			float hAngle = 2 * M_PI * ((float)xIndex / (float)SPHERE_H_SEGMENT_COUNT);
			float x = r * cos(hAngle);
			float z = r * sin(hAngle);
			unsigned int index = (xIndex + yIndex * SPHERE_H_SEGMENT_COUNT);
			unsigned int offset = 3 * index;
			//LOG("index:%d,x:%f ,y:%f, z:%f, hAngle:%f(%f), vAngle:%f(%f)\n", index, x, y, z, hAngle, hAngle * (360.0f / (2 * M_PI)), vAngle, vAngle * (360.0f / (2 * M_PI)));
			sphereVertexData[offset + 0] = x;
			sphereVertexData[offset + 1] = y;
			sphereVertexData[offset + 2] = z;
			sphereIndexData.push_back(index);
			sphereIndexData.push_back(((xIndex + 1) % SPHERE_H_SEGMENT_COUNT + yIndex * SPHERE_H_SEGMENT_COUNT));
			if (yIndex != SPHERE_V_SEGMENT_COUNT - 1) {
				sphereIndexData.push_back(index);
				sphereIndexData.push_back(index + SPHERE_H_SEGMENT_COUNT);
			}
		}
		//LOG("Layer %d Done\n", yIndex);
	}
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(sphereVertexData), sphereVertexData, GL_STATIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	sSphereIndexCount = sphereIndexData.size();
	LOG_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * sSphereIndexCount, &sphereIndexData[0], GL_STATIC_DRAW);

	// Setup stuff for Cylinder Drawing
	float cylinderVertexData[3 * 2 * CYLINDER_SEGMENT_COUNT];
	unsigned short cylinderIndexData[3 * 2 * CYLINDER_SEGMENT_COUNT];
	for (int i = 0; i < CYLINDER_SEGMENT_COUNT; i++) {
		float angle = (2 * M_PI) / (float)(CYLINDER_SEGMENT_COUNT)*i;
		float x = cos(angle);
		float z = sin(angle);
		cylinderVertexData[3 * 2 * i + 0] = x;
		cylinderVertexData[3 * 2 * i + 1] = -1;
		cylinderVertexData[3 * 2 * i + 2] = z;
		cylinderVertexData[3 * 2 * i + 3] = x;
		cylinderVertexData[3 * 2 * i + 4] = 1;
		cylinderVertexData[3 * 2 * i + 5] = z;

		cylinderIndexData[3 * 2 * i + 0] = 2 * i;
		cylinderIndexData[3 * 2 * i + 1] = 2 * i + 1;
		cylinderIndexData[3 * 2 * i + 2] = 2 * i;
		cylinderIndexData[3 * 2 * i + 3] = 2 * ((i + 1) % CYLINDER_SEGMENT_COUNT);
		cylinderIndexData[3 * 2 * i + 4] = 2 * i + 1;
		cylinderIndexData[3 * 2 * i + 5] = 2 * ((i + 1) % CYLINDER_SEGMENT_COUNT) + 1;
	}
	GLuint cylinderVertexBufferId;
	LOG_CALL(glGenBuffers, 1, &cylinderVertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, cylinderVertexBufferId);
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(cylinderVertexData), cylinderVertexData, GL_STATIC_DRAW);
	LOG_CALL(glGenVertexArrays, 1, &sCylinderVaoId);
	LOG_CALL(glBindVertexArray, sCylinderVaoId);
	LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
	LOG_CALL(glBindVertexArray, 0);
	LOG_CALL(glGenBuffers, 1, &sCylinderIndexBufferId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sCylinderIndexBufferId);
	LOG_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderIndexData), cylinderIndexData, GL_STATIC_DRAW);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);

	sInit = false;
}

void DebugRenderer::drawLine(const Vector& v1, const Vector& v2, const Matrix& transform) {
	if (sInit) {
		DebugRenderer::init();
	}

	// Upload Data
	float data[] = {
		v1.X,v1.Y,v1.Z,
		v2.X,v2.Y,v2.Z
	};
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, sLineVertexBufferId);
	LOG_CALL(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(data), data);

	// Setup Shader
	pShader->setModelViewProj(pCamera->getViewProj() * transform);
	pShader->setColor(1, 0, 0);
	pShader->activate();

	// Draw
	LOG_CALL(glBindVertexArray, sLineVaoId);
	LOG_CALL(glEnableVertexAttribArray, 0);
	LOG_CALL(glDrawArrays, GL_LINES, 0, 2);
	LOG_CALL(glDisableVertexAttribArray, 0);
	LOG_CALL(glBindVertexArray, 0);
}

void DebugRenderer::drawAABB(const AABB& aabb, const Matrix& transform)
{
	glDepthFunc(GL_ALWAYS);
	if (sInit) {
		DebugRenderer::init();
	}

	// Upload Data
	float data[] = {
		aabb.Min.X,aabb.Min.Y,aabb.Min.Z,
		aabb.Min.X,aabb.Max.Y,aabb.Min.Z,
		aabb.Min.X,aabb.Min.Y,aabb.Max.Z,
		aabb.Min.X,aabb.Max.Y,aabb.Max.Z,
		aabb.Max.X,aabb.Min.Y,aabb.Min.Z,
		aabb.Max.X,aabb.Max.Y,aabb.Min.Z,
		aabb.Max.X,aabb.Min.Y,aabb.Max.Z,
		aabb.Max.X,aabb.Max.Y,aabb.Max.Z
	};
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, sAABBVertexBufferId);
	LOG_CALL(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(data), data);

	// Setup Shader
	pShader->setModelViewProj(pCamera->getViewProj() * transform);
	pShader->setColor(1, 0, 0);
	pShader->activate();

	// Draw
	LOG_CALL(glBindVertexArray, sAABBVaoId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sAABBIndexBufferId);
	LOG_CALL(glEnableVertexAttribArray, 0);
	LOG_CALL(glDrawElements, GL_LINES, 24, GL_UNSIGNED_SHORT, 0);
	LOG_CALL(glDisableVertexAttribArray, 0);
	LOG_CALL(glBindVertexArray, 0);

	glDepthFunc(GL_LESS);
}
void DebugRenderer::drawUnitSphere(const Vector& pos, const Matrix& transform) {
	if (sInit) {
		DebugRenderer::init();
	}

	// Setup Shader
	pShader->setModelViewProj(pCamera->getViewProj() * transform);
	pShader->setColor(1, 0, 0);
	pShader->activate();

	// Draw
	LOG_CALL(glBindVertexArray, sSphereVaoId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sSphereIndexBufferId);
	LOG_CALL(glEnableVertexAttribArray, 0);
	LOG_CALL(glDrawElements, GL_LINES, sSphereIndexCount, GL_UNSIGNED_SHORT, 0);
	LOG_CALL(glDisableVertexAttribArray, 0);
	LOG_CALL(glBindVertexArray, 0);
}

void DebugRenderer::drawSphere(const Vector& pos, float radius, const Matrix& transform) {
	DebugRenderer::drawUnitSphere(pos, transform * Matrix().scale(radius));
}

void DebugRenderer::drawCylinder(const Matrix& transform)
{
	if (sInit) {
		DebugRenderer::init();
	}
	pShader->setModelViewProj(pCamera->getViewProj() * transform);
	pShader->setColor(1, 0, 0);
	pShader->activate();

	// Draw
	LOG_CALL(glBindVertexArray, sCylinderVaoId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, sCylinderIndexBufferId);
	LOG_CALL(glEnableVertexAttribArray, 0);
	LOG_CALL(glDrawElements, GL_LINES, 3 * 2 * CYLINDER_SEGMENT_COUNT, GL_UNSIGNED_SHORT, 0);
	LOG_CALL(glDisableVertexAttribArray, 0);
	LOG_CALL(glBindVertexArray, 0);
}

void DebugRenderer::setCamera(Camera* pCamera) {
	if (sDestroyCamera) {
		delete DebugRenderer::pCamera;
	}
	DebugRenderer::pCamera = pCamera;
	sDestroyCamera = false;
}