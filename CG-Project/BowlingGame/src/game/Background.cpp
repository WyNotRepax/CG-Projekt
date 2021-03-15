#include "Background.h"
#include "../Directories.h"

Background::Background() :GameObject() {
	mRadius = 0;
	mInverseMass = 0;
	pModel = new Model(MODEL_DIR"/bowlingbahn.dae");
	mDrag = 0;
	mPhysics = false;
}


