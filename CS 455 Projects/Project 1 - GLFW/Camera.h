#pragma once

#include "Window.h"
#include "cs455Utils.h"

class Camera
{
private:
	Window					*w;

	Vector455				position;
	Vector455				lookAt;
	Vector455				upVector;

public:
	/**
	 * Constructor/Destructor
	 */
	Camera(Window *w);
	
	~Camera(void);

	/**
	 * Public Methods
	 */
	void translateWorldSpace(float x, float y, float z);

	void translateCameraSpace(float right, float up, float back);

	void rotateWorldSpace(float theta, float x, float y, float z);

	void rotateCameraSpace(float theta, float right, float up, float back);

	void use();
};

