#pragma once

#include <GL/glfw.h>

#include "Keyboard.h"
#include "PointColor.h"
#include "CappedQueue.h"
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
	const static int OUTLINE_WIDTH	= 2;

	const static int MODE_OPENGL	= 1;
	const static int MODE_CS_455	= 2;
	
	const static int CS455_GL_NONE	= -1;

	//const static int LAST_SCENE	= 7;
	const static int LAST_SCENE		= 9;

	/**
	 * Other Members
	 */
	int drawMode;											// The current drawing mode, opengl, or CS_455

	float raster[RASTER_SIZE];								// Our personal raster buffer, 3 values per pixel.

	PointColor outline[WINDOW_HEIGHT][OUTLINE_WIDTH];		// Used to store data to fill triangles

	CappedQueue<PointColor>	pointQ;							// Stores the most recent points established with glVertex3i
	bool nIsOdd;											// Flag for drawing triangle strips.
	bool filling;											// Signifies that we are in the process of filling a polygon
		
	int sceneToRender;										// The current scene to render

	/**
	 * My OpenGL values
	 */
	Vector455						clearColor;			// Color to put in when the screen is cleared.
	Vector455						currentColor;		// The current color, changed by calling glColor3f
	GLenum							renderMode;			// The rendering mode used when glBegin was called
	GLfloat							lineWidth;			// The line width used when rending lines

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

	void plotLine(int x0, int y0, int x1, int y1, double r0, double g0, double b0, double r1, double g1, double b1);
	void plotLine(int x0, int y0, int x1, int y1, Vector455& startColor, Vector455& endColor);
	void plotLine(PointColor& pc0, PointColor& pc1);
	void plotLine(int x0, int y0, int x1, int y1);

	void clearOutline();
	void saveToOutline(PointColor& pc);
	void saveToOutline(int x, int y, double r, double g, double b);
	void fillOutline();
	bool fillableRenderingMode();

	/**
	 * My OpenGL wrappers
	 */
	void cs455_glEnable(GLenum cap);
	void cs455_glDisable(GLenum cap);
	void cs455_glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void cs455_glClear(GLbitfield mask);
	void cs455_glBegin(GLenum mode);
	void cs455_glEnd(void);
	void cs455_glVertex2i(GLint x, GLint y);
	void cs455_glVertex2f(GLfloat x, GLfloat y);
	void cs455_glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void cs455_glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void cs455_glColor3f(float r, float g, float b);
	void cs455_glLineWidth(float width);
	void cs455_glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void cs455_glLoadIdentity(void);
	void cs455_glPushMatrix(void);
	void cs455_glPopMatrix(void);
	void cs455_glLoadMatrixd(const GLdouble *m);
	void cs455_glMultMatrixd(const GLdouble *m);
	void cs455_glRotatef(GLfloat theta, GLfloat x, GLfloat y, GLfloat z);
	void cs455_glTranslatef(GLfloat x, GLfloat y, GLfloat z);
	void cs455_glScalef(GLfloat x, GLfloat y, GLfloat z);
	void cs455_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);

	/**
	 * Project Rendering Methods
	 */
	void p2_redraw(void);
	void p2_renderPoints(void);
	void p2_renderLines(void);
	void p2_renderPolygons(void);
	void p2_renderLineStrip(void);
	void p2_renderLineLoop(void);
	void p2_renderTriangleStrip(void);
	void p2_renderTriangleFan(void);
	void p2_renderQuads(void);
	void p2_renderQuadStrip(void);
	void p2_renderFatLines(void);

	void p3_redraw(void);
	void p3_renderViewport(void);
	void p3_render1OverW(void);
	void p3_renderDepthBuffer(void);
	void p3_renderEasyPointClipping(void);
	void p3_renderMatrixManips(void);
	void p3_renderMatrixStacks(void);
	void p3_renderRotate(void);
	void p3_renderTranslate(void);
	void p3_renderScale(void);
	void p3_renderOrtho(void);
	void p3_tree(int depth);
};

