#pragma once

#include <GL/glfw.h>

#include "Keyboard.h"

/**
 * Window class
 * Encapsulates all the glfw window setup for this program
 *
 * Jan. 9, 2012
 */
class Window
{
private:
	/**
	 * Class constants
	 */
	const static int WINDOW_WIDTH	= 640;
	const static int WINDOW_HEIGHT	= 480;

	const static int DIMENSIONS		= 3;
	const static int RASTER_SIZE	= WINDOW_WIDTH * WINDOW_HEIGHT * DIMENSIONS;
	
	const static int MODE_OPENGL	= 1;
	const static int MODE_CS_455	= 2;
	
	/**
	 * Other Members
	 */
	int drawMode;					// The current drawing mode, opengl, or CS_455

	float raster[RASTER_SIZE];		// Our personal raster buffer, 3 values per pixel.

public:
	/**
	 * Constructor
	 */
	Window(void);

	/**
	 * Destructor
	 */
	~Window(void);

	/**
	 * Other Methods
	 */
	bool Open(void);

	void EnterMainLoop(void);

private:
	void redraw(void);
	
	void checkRenderingMode(void);
};

