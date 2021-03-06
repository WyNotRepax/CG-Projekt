#pragma once

#include "../math/Matrix.h"

class Camera
{
private:
	Matrix viewProj;
	Matrix view;
	Matrix proj;

	inline void updateViewProj();
public:
	Camera();
	Camera(const Matrix& view, const Matrix& proj);

	Matrix getView() const;
	void setView(const Matrix& m);
	
	Matrix getProj() const;
	void setProj(const Matrix& m);

	Matrix getViewProj() const;
};

