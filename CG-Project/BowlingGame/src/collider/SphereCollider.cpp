#include "SphereCollider.h"
#include "../renderer/DebugRenderer.h"

SphereCollider::SphereCollider():mRelativeTransform(Matrix().identity()) {

}

void SphereCollider::debugDraw(const Matrix & transform) const
{
	DebugRenderer::drawSphere(Vector(0, 0, 0), 1,transform * mRelativeTransform);
}

bool SphereCollider::collidesWith(const Collider* pOther) const
{
	return false;
}
