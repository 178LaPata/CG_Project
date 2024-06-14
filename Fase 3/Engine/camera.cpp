#include "Headers/camera.h"

Ponto* Camera::getPosition()
{
    return this->position;
}

Ponto* Camera::getLookAt()
{
    return this->lookAt;
}

Ponto* Camera::getUp()
{
    return this->up;
}

float Camera::getFov()
{
    return this->fov;
}

float Camera::getNear()
{
    return this->near;
}

float Camera::getFar()
{
    return this->far;
}

void Camera::setPosition(Ponto* p)
{
    this->position = p;
}

void Camera::setLookAt(Ponto* p)
{
    this->lookAt = p;
}

void Camera::setUp(Ponto* p)
{
    this->up = p;
}

void Camera::setFov(float fov)
{
    this->fov = fov;
}

void Camera::setNear(float near)
{
    this->near = near;
}

void Camera::setFar(float far)
{
    this->far = far;
}

void Camera::setCamera(Ponto* position, Ponto* lookAt, Ponto* up, float fov, float near, float far)
{
    this->position = position;
    this->lookAt = lookAt;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
}

