#include <GL/glfw.h>

#include "Window.h"
#include "cs455Utils.h"

void GLFWCALL ResizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1); // only for project 2; delete thereafter!

	return;
}

Window::Window(void)
{
	glfwInit();

	drawMode = MODE_OPENGL;

	/**
	 * Set Defaults
	 */
	renderMode = CS455_GL_NONE;

	clearColor.Zero();			// Default clear color = black

	currentColor.r() = 1.0f;	// Default render color = white
	currentColor.g() = 1.0f;	
	currentColor.b() = 1.0f;

	lineWidth = 1.0f;			// Default line width = 1 px
}

Window::~Window(void)
{
	// Close window and terminate GLFW
	glfwTerminate();
}

bool Window::Open(void)
{
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ))
		return false;

	glfwSetWindowSizeCallback(ResizeCallback);
	glfwSetKeyCallback(KeyboardCallback);

	return true;
}

void Window::EnterMainLoop(void)
{
	bool running = true;

	while (running)
	{
		checkRenderingMode();

		redraw();

		if (KEY_DOWN(GLFW_KEY_ESC))
			running = false;
	}
}

/**********************************************
 * Private Methods
 */

void Window::redraw(void)
{
	cs455_glClearColor(0, 0, 0, 1);
	cs455_glClear(GL_COLOR_BUFFER_BIT);
	
	if (KEY_DOWN('Q'))
	{
		p2_renderPoints();
		p2_renderLines();
		p2_renderPolygons();
	}
	else if (KEY_DOWN('W'))
		p2_renderLineStrip();
	else if (KEY_DOWN('E'))
		p2_renderLineLoop();
	else if (KEY_DOWN('R'))
		p2_renderTriangleStrip();
	else if (KEY_DOWN('T'))
		p2_renderTriangleFan();
	else if (KEY_DOWN('Y'))
		p2_renderQuads();
	else if (KEY_DOWN('U'))
		p2_renderQuadStrip();
	else if (KEY_DOWN('I'))
		p2_renderFatLines();

	if (drawMode == MODE_CS_455)
		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, raster);

	glFlush();

	glfwSwapBuffers();
}

void Window::waterMarkMine(void)
{
	for (int x=620; x<640; x++)
	{
		for (int y=460; y<480; y++)
			setPixel(x, y, 1, 1, 1);
	}
}

void Window::checkRenderingMode(void)
{
	if (KEY_DOWN('1'))
		drawMode = MODE_OPENGL;
	else if (KEY_DOWN('2'))
		drawMode = MODE_CS_455;

	return;
}

void Window::setPixel(int x, int y, double r, double g, double b)
{
	raster[((y*640) + x)*3 + 0] = (float)r;
	raster[((y*640) + x)*3 + 1] = (float)g;
	raster[((y*640) + x)*3 + 2] = (float)b;

	return;
}

/**
 * My OpenGL Wrappers
 */
void Window::cs455_glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);

	clearColor.r() = r;
	clearColor.g() = g;
	clearColor.b() = b;
	clearColor.a() = a;
}

void Window::cs455_glClear(GLbitfield mask)
{
	glClear(mask);

	if ((mask & GL_COLOR_BUFFER_BIT) != 0)
	{
		for (int x=0; x<WINDOW_WIDTH; x++)
		{
			for (int y=0; y<WINDOW_HEIGHT; y++)
				setPixel(x, y, clearColor(0), clearColor(1), clearColor(2));
		}
	}

	waterMarkMine();

	return;
}

void Window::cs455_glBegin(GLenum mode)
{
	glBegin(mode);

	renderMode = mode;
}

void Window::cs455_glEnd()
{
	glEnd();

	renderMode = CS455_GL_NONE;
}

void Window::cs455_glVertex2i(GLint x, GLint y)
{
	glVertex2i(x, y);

	switch (renderMode)
	{
		case GL_POINTS:
			setPixel(x, y, currentColor.r(), currentColor.g(), currentColor.b());

		case GL_LINE:

		case GL_LINE_STRIP:

		case GL_LINE_LOOP:

		case GL_TRIANGLE_STRIP:

		case GL_TRIANGLE_FAN:
		case GL_POLYGON:

		case GL_QUADS:

		case GL_QUAD_STRIP:

		case CS455_GL_NONE:
		default: break;
	}

	return;
}

void Window::cs455_glColor3f(float r, float g, float b)
{
	glColor3f(r, g, b);

	currentColor.r() = r;
	currentColor.g() = g;
	currentColor.b() = b;

	return;
}

void Window::cs455_glLineWidth(float width)
{
	glLineWidth(width);

	lineWidth = width;

	return;
}

/**
 * Project rendering methods
 */
void Window::p2_renderPoints(void)
{	
	cs455_glBegin(GL_POINTS);

	for(float theta=0, radius=60.0f; radius>1.0f; theta+=0.1f, radius-=0.3f)
	{
		cs455_glColor3f(radius/60.0f,0.3f,1.0f-(radius/60.0f));
		cs455_glVertex2i((int)(80.0f+radius*cos(theta)),(int)(400.0f+radius*sin(theta)));
	}
	
	cs455_glEnd();
}

void Window::p2_renderLines(void)
{
	cs455_glBegin(GL_LINES);

	for(int i=0; i<=8; i++)
	{
		cs455_glColor3f(1,0,0);
		cs455_glVertex2i(100,250);
		cs455_glVertex2i(100 + 10*i, 330);
		cs455_glColor3f(0,1,0);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(0,1,1);
		cs455_glVertex2i(100 - 10*i, 330);
		cs455_glVertex2i(100,250);
		cs455_glVertex2i(180, 250 + 10*i);
		cs455_glVertex2i(100,250);
		cs455_glVertex2i(180, 250 - 10*i);
	}

	cs455_glEnd();
}

void Window::p2_renderPolygons(void)
{
	cs455_glBegin(GL_TRIANGLES);
 
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(500,400);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(600,400);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(550,460);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(300,460);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(270,310);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(550,350);

	cs455_glEnd();
}

void Window::p2_renderLineStrip(void)
{
	cs455_glBegin(GL_LINE_STRIP);

	cs455_glColor3f(0.42,0.27,0.11);
	cs455_glVertex2i(250,30);
	cs455_glVertex2i(270,60);
	cs455_glVertex2i(270,210);
	cs455_glColor3f(0.04,0.70,0.02);
	cs455_glVertex2i(230,230);
	cs455_glVertex2i(220,270);
	cs455_glVertex2i(220,310);
	cs455_glVertex2i(250,340);
	cs455_glVertex2i(275,360);
	cs455_glVertex2i(325,360);
	cs455_glVertex2i(350,340);
	cs455_glVertex2i(380,310);
	cs455_glVertex2i(380,270);
	cs455_glVertex2i(370,230);
	cs455_glColor3f(0.42,0.27,0.11);
	cs455_glVertex2i(330,210);
	cs455_glVertex2i(330,60);
	cs455_glVertex2i(350,30);

	cs455_glEnd();
}

void Window::p2_renderLineLoop(void)
{
	cs455_glBegin(GL_LINE_LOOP);

	cs455_glColor3f(0.42,0.27,0.11);
	cs455_glVertex2i(250,30);
	cs455_glVertex2i(270,60);
	cs455_glVertex2i(270,210);
	cs455_glColor3f(0.04,0.70,0.02);
	cs455_glVertex2i(230,230);
	cs455_glVertex2i(220,270);
	cs455_glVertex2i(220,310);
	cs455_glVertex2i(250,340);
	cs455_glVertex2i(275,360);
	cs455_glVertex2i(325,360);
	cs455_glVertex2i(350,340);
	cs455_glVertex2i(380,310);
	cs455_glVertex2i(380,270);
	cs455_glVertex2i(370,230);
	cs455_glColor3f(0.42,0.27,0.11);
	cs455_glVertex2i(330,210);
	cs455_glVertex2i(330,60);
	cs455_glVertex2i(350,30);

	cs455_glEnd();
}

void Window::p2_renderTriangleStrip(void)
{
	cs455_glBegin(GL_TRIANGLE_STRIP);

	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(40,70);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(40,390);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(130,30);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(130,350);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(330,80);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(330,400);
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(480,40);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(530,330);

	cs455_glEnd();
}

void Window::p2_renderTriangleFan(void)
{
	cs455_glBegin(GL_TRIANGLE_FAN);

	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(250,170);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(400,140);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(300,50);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(175,55);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(100,170);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(175,285);
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(300,290);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(400,200);

	cs455_glEnd();
}

void Window::p2_renderQuads(void)
{
	cs455_glBegin(GL_QUADS);

	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(40,70);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(40,390);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(130,350);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(130,30);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(330,80);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(330,400);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(530,330);
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(480,40);

	cs455_glEnd();
}

void Window::p2_renderQuadStrip(void)
{
	cs455_glBegin(GL_QUAD_STRIP);

	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(40,70);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(40,390);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(130,30);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(130,350);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(330,80);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(330,400);
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(480,40);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(530,330);

	cs455_glEnd();
}

void Window::p2_renderFatLines(void)
{
	cs455_glLineWidth(5);
	
	cs455_glBegin(GL_LINES);
 
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(200,270);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(225,300);
	cs455_glVertex2i(225,300);
	cs455_glColor3f(1,1,0);
	cs455_glVertex2i(255,300);
	cs455_glVertex2i(255,300);
	cs455_glColor3f(0,0,1);
	cs455_glVertex2i(280,270);
	cs455_glVertex2i(280,270);
	cs455_glColor3f(1,0,1);
	cs455_glVertex2i(280,230);
	cs455_glVertex2i(280,230);
	cs455_glColor3f(0,1,1);
	cs455_glVertex2i(240,190);
	cs455_glVertex2i(240,190);
	cs455_glColor3f(1,0,0);
	cs455_glVertex2i(240,160);
	cs455_glVertex2i(240,150);
	cs455_glColor3f(0,1,0);
	cs455_glVertex2i(240,145);

	cs455_glEnd();

	cs455_glLineWidth(1);
}
