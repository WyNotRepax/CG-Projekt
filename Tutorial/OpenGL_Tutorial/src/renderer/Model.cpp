#include "Model.h"

#include "../shader/ConstantShader.h"

Model::Model() {
	pShader = ConstantShader::GetInstance();
}

void Model::draw(const Camera*)const {
	pShader->activate();
}
