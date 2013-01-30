#include <iostream>

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

	prevPtSet = false;
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
	
	if (sceneToRender == 0)
	{
		p2_renderPoints();
		p2_renderLines();
		p2_renderPolygons();
	}
	else if (sceneToRender == 1)
		p2_renderLineStrip();
	else if (sceneToRender == 2)
		p2_renderLineLoop();
	else if (sceneToRender == 3)
		p2_renderTriangleStrip();
	else if (sceneToRender == 4)
		p2_renderTriangleFan();
	else if (sceneToRender == 5)
		p2_renderQuads();
	else if (sceneToRender == 6)
		p2_renderQuadStrip();
	else if (sceneToRender == 7)
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

	if (KEY_DOWN(GLFW_KEY_LEFT))
		sceneToRender--;
	else if (KEY_DOWN(GLFW_KEY_RIGHT))
		sceneToRender++;

	if (sceneToRender < 0)
		sceneToRender = LAST_SCENE;
	else if (sceneToRender > LAST_SCENE)
		sceneToRender = 0;

	return;
}

void Window::setPixel(int x, int y, double r, double g, double b)
{
	raster[((y*WINDOW_WIDTH) + x)*3 + 0] = (float)r;
	raster[((y*WINDOW_WIDTH) + x)*3 + 1] = (float)g;
	raster[((y*WINDOW_WIDTH) + x)*3 + 2] = (float)b;

	return;
}

void Window::plotLine(int x0, int y0, int x1, int y1)
{
	if (x1 - x0 == 0) // Vertical Lines
	{
		if (y0 > y1)
		{
			while (y0 >= y1)
				setPixel(x0, y0--, currentColor.r(), currentColor.g(), currentColor.b());
		}
		else
		{
			while (y0 < y1)
				setPixel(x0, y0++, currentColor.r(), currentColor.g(), currentColor.b());
		}

		return;
	}
	else if (y1 - y0 == 0) // Horizontal lines
	{
		if (x0 > x1)
		{
			while (x0 >= x1)
				setPixel(x0--, y0, currentColor.r(), currentColor.g(), currentColor.b());
		}
		else
		{
			while (x0 < x1)
				setPixel(x0++, y0, currentColor.r(), currentColor.g(), currentColor.b());
		}

		return;
	}

	// Other Lines, we always draw them left to right
	int startX, startY, endX, endY;
	int stepX = 1, stepY = 0;

	if (x0 < x1)
	{
		startX = x0;
		startY = y0;
		endX = x1;
		endY = y1;
	}
	else
	{
		startX = x1;
		startY = y1;
		endX = x0;
		endY = y0;
	}
	
	if (startY > endY) // Going downhill on screen
		stepY = -1;
	else
		stepY = 1;

	int dx = abs(endX - startX);
	int dy = abs(endY - startY);
	int d2y = 2 * dy;
	int d2x = 2 * dx;

	int d2xy = 2 * (dy - dx);
	int d2 = d2y;
	if (dy > dx)
	{
		d2xy = 2 * (dx - dy);
		d2 = d2x;
	}

	int p0 = d2 - dx;
	int deltaStep = dx > dy ? abs(endX - startX) : abs(endY - startY);

	setPixel(startX, startY, currentColor.r(), currentColor.g(), currentColor.b());
	for (int step=1; step<deltaStep; step++)
	{
		if (p0 < 0)
		{
			if (dx > dy)
				startX += stepX;
			else
				startY += stepY;

			p0 += d2;
		}
		else
		{
			startX += stepX;
			startY += stepY;

			p0 += d2xy;
		}

		setPixel(startX, startY, currentColor.r(), currentColor.g(), currentColor.b());
	}

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

	// Eigen is SUPER slow if you do thousands of lookups like this each frame,
	// so I'm caching them here to speed things up. A lot.
	double clearR = clearColor(0);
	double clearG = clearColor(1);
	double clearB = clearColor(2);

	if ((mask & GL_COLOR_BUFFER_BIT) != 0)
	{
		for (int x=0; x<WINDOW_WIDTH; x++)
		{
			for (int y=0; y<WINDOW_HEIGHT; y++)
				setPixel(x, y, clearR, clearG, clearB);
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
			break;

		case GL_LINES:
			if (prevPtSet)
			{
				plotLine(prevPtX, prevPtY, x, y);
				prevPtSet = false;
			}
			else
			{
				prevPtSet = true;
				prevPtX = x;
				prevPtY = y;
			}
			break;

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

	if (!prevPtSet)
		prevColor = currentColor;

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

	cs455_glColor3f(1,1,1);
	cs455_glVertex2i(0,0);
	cs455_glVertex2i(100, 50);

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
		cs455_glColor3f(0,0,1);
		cs455_glVertex2i(180, 250 + 10*i);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(1,0,1);
		cs455_glVertex2i(180, 250 - 10*i);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(1,1,1);
		cs455_glVertex2i(20, 250 + 10 * i);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(1,1,0);
		cs455_glVertex2i(20, 250 - 10 * i);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(0,1,0);
		cs455_glVertex2i(100 + 10 * i, 170);
		cs455_glVertex2i(100,250);
		cs455_glColor3f(1,0,0);
		cs455_glVertex2i(100 - 10 * i, 170);
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

	cs455_glColor3f(0.42f,0.27f,0.11f);
	cs455_glVertex2i(250,30);
	cs455_glVertex2i(270,60);
	cs455_glVertex2i(270,210);
	cs455_glColor3f(0.04f,0.70f,0.02f);
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
	cs455_glColor3f(0.42f,0.27f,0.11f);
	cs455_glVertex2i(330,210);
	cs455_glVertex2i(330,60);
	cs455_glVertex2i(350,30);

	cs455_glEnd();
}

void Window::p2_renderLineLoop(void)
{
	cs455_glBegin(GL_LINE_LOOP);

	cs455_glColor3f(0.42f,0.27f,0.11f);
	cs455_glVertex2i(250,30);
	cs455_glVertex2i(270,60);
	cs455_glVertex2i(270,210);
	cs455_glColor3f(0.04f,0.70f,0.02f);
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
	cs455_glColor3f(0.42f,0.27f,0.11f);
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
