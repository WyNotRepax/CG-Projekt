#include "CylinderCollider.h"
#include "../renderer/DebugRenderer.h"
void CylinderCollider::debugDraw(const Matrix& transform) const
{
	DebugRenderer::drawCylinder(transform * mRelativeTransform);
}

bool CylinderCollider::collidesWith(const Collider* pOther) const
{
	return false;
}
