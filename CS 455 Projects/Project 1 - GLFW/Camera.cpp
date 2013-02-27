#include "Camera.h"
#include "Window.h"

Camera::Camera(Window *w)
{
	this->w = w;

	position.Zero();
	
	lookAt.Zero();
	lookAt.z() = -1.0f;

	upVector.Zero();
	upVector.y() = 1.0f;
}

Camera::~Camera(void)
{

}

void Camera::translateWorldSpace(float x, float y, float z)
{
	position.x() = x;
	position.y() = y;
	position.z() = z;
}

void Camera::translateCameraSpace(float right, float up, float back)
{
	position.x() += right;
	position.y() += up;
	position.z() += back;
}

// Rotate the camera around the world
void Camera::rotateWorldSpace(float theta, float x, float y, float z)
{

}

// Rotates just camera itself. (Rotate lookAt point around camera)
void Camera::rotateCameraSpace(float theta, float right, float up, float back)
{

}

void Camera::use()
{
	w->cs455_glMatrixMode(Window::CS455_GL_PROJECTION);
	w->cs455_glLoadIdentity(); // In Project 4, load a perspective matrix here instead of the identity.

	w->cs455_glMatrixMode(Window::CS455_GL_MODELVIEW);
	// Orient the camera...

}
