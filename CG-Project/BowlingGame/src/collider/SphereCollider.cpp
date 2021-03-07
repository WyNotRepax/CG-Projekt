#include "SphereCollider.h"
#include "../renderer/DebugRenderer.h"

SphereCollider::SphereCollider():mRadius(1),mRelativeTransform(Matrix().identity()) {

}

void SphereCollider::debugDraw(Camera* pCamera, const Matrix & transform) const
{
	DebugRenderer::drawSphere(Vector(0, 0, 0), mRadius,transform * mRelativeTransform);
}

bool SphereCollider::collidesWith(const Collider& pOther) const
{
	return false;
}
