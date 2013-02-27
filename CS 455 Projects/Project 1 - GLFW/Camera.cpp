#include "Camera.h"
#include "Window.h"

Camera::Camera(Window *w)
{
	this->w = w;

	transformation = Matrix455::Identity();
}

Camera::~Camera(void)
{

}

void Camera::translateWorldSpace(float x, float y, float z)
{
	transformation(0, 3) = x;
	transformation(1, 3) = y;
	transformation(2, 3) = z;
}

void Camera::translateCameraSpace(float right, float up, float back)
{
	transformation(0, 3) += right;
	transformation(1, 3) += up;
	transformation(2, 3) += back;
}

// Rotate the camera around the world
void Camera::rotateWorldSpace(float theta, float x, float y, float z)
{
	/*temp = Matrix455::Identity();
	
	theta *= (float)(M_PI / 180);

	double cosTheta = cos(theta);
	double sinTheta = sin(theta);

	temp(0, 0) = (float)(pow(x, 2) * (1-cosTheta) + cosTheta);
	temp(0, 1) = (float)(x * y * (1-cosTheta) - z * sinTheta);
	temp(0, 2) = (float)(x * z * (1-cosTheta) + y * sinTheta);

	temp(1, 0) = (float)(y * x * (1-cosTheta) + z * sinTheta);
	temp(1, 1) = (float)(pow(y, 2) * (1-cosTheta) + cosTheta);
	temp(1, 2) = (float)(y * z * (1-cosTheta) - x * sinTheta);
	
	temp(2, 0) = (float)(z * x * (1-cosTheta) - y * sinTheta);
	temp(2, 1) = (float)(z * y * (1-cosTheta) + x * sinTheta);
	temp(2, 2) = (float)(pow(z, 2) * (1-cosTheta) + cosTheta);

	activeMatrix[currentMatrix] *= temp;*/
}

// Rotates just camera itself. (Translate the camera to origin, rotate, translate back)
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
