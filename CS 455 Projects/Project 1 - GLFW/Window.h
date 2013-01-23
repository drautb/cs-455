#pragma once

#include <GL/glfw.h>

#include "Keyboard.h"
#include "cs455Utils.h"

void GLFWCALL ResizeCallback(int width, int height);

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
	
	const static int CS455_GL_NONE	= -1;

	/**
	 * Other Members
	 */
	int drawMode;					// The current drawing mode, opengl, or CS_455

	float raster[RASTER_SIZE];		// Our personal raster buffer, 3 values per pixel.

	/**
	 * My OpenGL values
	 */
	Vector455						clearColor;			// Color to put in when the screen is cleared.
	Vector455						currentColor;		// The current color, changed by calling glColor3f
	GLenum							renderMode;			// The rendering mode used when glBegin was called

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
	void waterMarkMine(void);
	
	void checkRenderingMode(void);

	void setPixel(int x, int y, double r, double g, double b);

	/**
	 * My OpenGL wrappers
	 */
	void cs455_glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void cs455_glClear(GLbitfield mask);
	void cs455_glBegin(GLenum mode);
	void cs455_glEnd(void);
	void cs455_glVertex2i(GLint x, GLint y);
	void cs455_glColor3f(float r, float g, float b);

	/**
	 * Project Rendering Methods
	 */
	void p2_renderPoints(void);
	void p2_renderLines(void);
	void p2_renderPolygons(void);
	void p2_renderLineStrip(void);
	void p2_renderLineLoop(void);
	void p2_renderTriangleStrip(void);
	void p2_renderTriangleFan(void);
	void p2_renderQuads(void);
	void p2_renderQuadStrip(void);
};

