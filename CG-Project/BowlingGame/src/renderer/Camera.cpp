#include "Camera.h"

inline void Camera::updateViewProj()
{
    viewProj = proj * view;
}

Camera::Camera() :Camera(Matrix().identity(), Matrix().identity())
{
}

Camera::Camera(const Matrix& view, const Matrix& proj):view(view),proj(proj)
{
    updateViewProj();
}

Matrix Camera::getView()
{
    return view;
}

void Camera::setView(const Matrix& m)
{
    view = m;
    updateViewProj();
}

Matrix Camera::getProj()
{
    return Matrix();
}

void Camera::setProj(const Matrix& m)
{
    proj = m;
    updateViewProj();
}

Matrix Camera::getViewProj()
{
    return viewProj;
}
