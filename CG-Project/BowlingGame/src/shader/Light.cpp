#include "Light.h"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

Matrix Light::getView() const
{
	assert(Type == SPOT);
	assert(Direction.magnitudeSquared() > 0);
	if (Direction.cross(Vector(0, 1, 0)).magnitudeSquared() == 0) {
		return Matrix().lookAt(Position + Direction, Vector(1, 0, 0), Position);
	}
	return Matrix().lookAt(Position + Direction, Vector(0, 1, 0), Position);
}

Matrix Light::getProj() const {
	return Matrix().perspective(SpotRadius.Y * 2 , 1.0f, 0.001f, 10);
}
