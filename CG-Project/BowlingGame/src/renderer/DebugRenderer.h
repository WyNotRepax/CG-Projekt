#pragma once

#include <vector>
#include "../math/Vector.h"
#include "Camera.h"
#include "GL/glew.h"
#include "../shader/ConstantShader.h"
#include "../math/AABB.h"

class DebugRenderer
{
private:
	static bool sInit;
	
	static GLuint sLineVertexBufferId;
	static GLuint sLineVaoId;

	static GLuint sAABBVertexBufferId;
	static GLuint sAABBIndexBufferId;
	static GLuint sAABBVaoId;

	static GLuint sSphereVertexBufferId;
	static GLuint sSphereIndexBufferId;
	static GLuint sSphereVaoId;
	static unsigned int sSphereIndexCount;

	static Camera* pCamera;
	static bool sDestroyCamera;
	static ConstantShader* pShader;

	static GLuint sCylinderVaoId;
	static GLuint sCylinderIndexBufferId;
	static unsigned int sCylinderIndexCount;

	static void init();

public:
	static void setCamera(Camera* pCamera);
	static void drawLine(const Vector& v1, const Vector& v2, const Matrix& transform = Matrix().identity());
	static void drawAABB(const AABB& aabb, const Matrix& transform = Matrix().identity());
	static void drawUnitSphere(const Vector& pos, const Matrix& transform = Matrix().identity());
	static void drawSphere(const Vector& pos, float radius, const Matrix& transform = Matrix().identity());
	static void drawCylinder(const Matrix& transform = Matrix().identity());
};

