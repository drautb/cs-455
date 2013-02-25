#include <iostream>

#include <cmath>

#include <GL/glfw.h>

#include "Window.h"
#include "cs455Utils.h"
#include "P2.h"
#include "P3.h"

void GLFWCALL ResizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glOrtho(0, width, 0, height, -1, 1); // only for project 2; delete thereafter!

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
	nIsOdd = true;
	filling = false;

	clearColor.Zero();			// Default clear color = black

	currentColor.r() = 1.0f;	// Default render color = white
	currentColor.g() = 1.0f;	
	currentColor.b() = 1.0f;
	currentColor.a() = 1.0f;

	lineWidth = 1.0f;			// Default line width = 1 px

	currentMatrix = CS455_GL_MODELVIEW;
	for (int s=0; s<MATRIX_MODE_COUNT; s++)
	{
		while (!matrixStack[s].empty()) matrixStack[s].pop();
		activeMatrix[s].setIdentity();
		matrixStack[s].push(activeMatrix[s]);
	}

	sceneToRender = 0;
}

Window::~Window(void)
{
	// Close window and terminate GLFW
	glfwTerminate();
}

bool Window::Open(void)
{
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 1, 0, GLFW_WINDOW ))
		return false;

	glfwSetWindowSizeCallback(ResizeCallback);
	glfwSetKeyCallback(KeyboardCallback);

	// P2 ONLY
	//glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

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
	cs455_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//P2::redraw(this);
	P3::redraw(this);

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
	if (KEY_HIT('1'))
		drawMode = MODE_OPENGL;
	else if (KEY_HIT('2'))
		drawMode = MODE_CS_455;

	if (KEY_HIT(GLFW_KEY_LEFT))
		sceneToRender--;
	else if (KEY_HIT(GLFW_KEY_RIGHT))
		sceneToRender++;

	return;
}

void Window::setPixel(int x, int y, double r, double g, double b)
{
	raster[((y*WINDOW_WIDTH) + x)*3 + 0] = (float)r;
	raster[((y*WINDOW_WIDTH) + x)*3 + 1] = (float)g;
	raster[((y*WINDOW_WIDTH) + x)*3 + 2] = (float)b;

	return;
}

void Window::plotLine(int x0, int y0, int x1, int y1, double r0, double g0, double b0, double r1, double g1, double b1)
{
	// Color calculations
	double red = r0, green = g0, blue = b0;
	double dRed, dGreen, dBlue;

	int dPixels;
	if (abs(x1 - x0) > abs(y1 - y0))
		dPixels = abs(x1 - x0);
	else
		dPixels = abs(y1 - y0);

	dRed = (r1 - r0) / dPixels;
	dGreen = (g1 - g0) / dPixels;
	dBlue = (b1 - b0) / dPixels;

	if (x1 - x0 == 0) // Vertical Lines
	{
		if (y0 > y1)
		{
			while (y0 >= y1)
			{
				saveToOutline(x0, y0, red, green, blue);
				if (lineWidth > 1.0f)
				{
					for (int x=x0-(int)(lineWidth/2.0f); x<x0+(int)(lineWidth/2); x++)
						setPixel(x, y0, red, green, blue);
					y0--;
				}
				else
					setPixel(x0, y0--, red, green, blue);
				red += dRed;
				green += dGreen;
				blue += dBlue;
			}
		}
		else
		{
			while (y0 < y1)
			{
				saveToOutline(x0, y0, red, green, blue);
				if (lineWidth > 1.0f)
				{
					for (int x=x0-(int)(lineWidth/2.0f); x<x0+(int)(lineWidth/2); x++)
						setPixel(x, y0, red, green, blue);
					y0++;
				}
				else
					setPixel(x0, y0++, red, green, blue);
				red += dRed;
				green += dGreen;
				blue += dBlue;
			}
		}

		return;
	}
	else if (y1 - y0 == 0) // Horizontal lines
	{
		if (x0 > x1)
		{
			while (x0 >= x1)
			{
				if (lineWidth > 1.0f)
				{
					for (int y=y0-(int)(lineWidth/2.0f); y<y0+(int)(lineWidth/2); y++)
						setPixel(x0, y, red, green, blue);
					x0--;
				}
				else
					setPixel(x0--, y0, red, green, blue);
				red += dRed;
				green += dGreen;
				blue += dBlue;
			}
		}
		else
		{
			while (x0 < x1)
			{
				if (lineWidth > 1.0f)
				{
					for (int y=y0-(int)(lineWidth/2.0f); y<y0+(int)(lineWidth/2); y++)
						setPixel(x0, y, red, green, blue);
					x0++;
				}
				else
					setPixel(x0++, y0, red, green, blue);
				red += dRed;
				green += dGreen;
				blue += dBlue;
			}
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

		red = r1;
		green = g1;
		blue = b1;
		dRed = (r0 - r1) / dPixels;
		dGreen = (g0 - g1) / dPixels;
		dBlue = (b0 - b1) / dPixels;
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

	saveToOutline(startX, startY, red, green, blue);
	setPixel(startX, startY, red, green, blue);
	for (int step=1; step<deltaStep; step++)
	{
		if (p0 < 0)
		{
			if (dx > dy)
				startX += stepX;
			else
			{
				startY += stepY;
				saveToOutline(startX, startY, red, green, blue);
			}

			p0 += d2;
		}
		else
		{
			startX += stepX;
			startY += stepY;
			saveToOutline(startX, startY, red, green, blue);

			p0 += d2xy;
		}

		if (lineWidth > 1.0f)
		{
			if (dx > dy) // change in x is greater, so the line should be thicker in the y
			{
				for (int y=startY-(int)(lineWidth/2.0f); y<startY+(int)(lineWidth/2); y++)
					setPixel(startX, y, red, green, blue);
			} 
			else // change in y is greater, so the line should be thicker in the x
			{
				for (int x=startX-(int)(lineWidth/2.0f); x<startX+(int)(lineWidth/2); x++)
					setPixel(x, startY, red, green, blue);
			}
		}
		else
			setPixel(startX, startY, red, green, blue);
		red += dRed;
		green += dGreen;
		blue += dBlue;
	}

	return;
}

void Window::plotLine(int x0, int y0, int x1, int y1, Vector455& startColor, Vector455& endColor)
{
	plotLine(x0, y0, x1, y1, startColor.r(), startColor.g(), startColor.b(), endColor.r(), endColor.g(), endColor.b());
}

void Window::plotLine(PointColor& pc0, PointColor& pc1)
{
	plotLine(pc0.x, pc0.y, pc1.x, pc1.y, pc0.color, pc1.color);
}

void Window::plotLine(int x0, int y0, int x1, int y1)
{
	plotLine(x0, y0, x1, y1, currentColor, currentColor);
}

void Window::clearOutline()
{
	for (int y=0; y<WINDOW_HEIGHT; y++)
	{
		outline[y][0].Reset();
		outline[y][1].Reset();
	}
}

void Window::saveToOutline(PointColor& pc)
{
	if (!fillableRenderingMode())
		return;

	if (outline[pc.y][0].x == -1)
		outline[pc.y][0] = pc;
	else if (outline[pc.y][1].x == -1)
		outline[pc.y][1] = pc;
	else
	{
		// There's a contest, both buffer slots are full at this y position, so we 
		// need to determine which one should be discarded. We'll keep the two that
		// have the greatest dx
		if (abs(pc.x - outline[pc.y][0].x) > abs(outline[pc.y][1].x - outline[pc.y][0].x))
			outline[pc.y][1] = pc;
	}
}

void Window::saveToOutline(int x, int y, double r, double g, double b)
{
	if (!fillableRenderingMode())
		return;

	Vector455 color;
	color.r() = (float)r;
	color.g() = (float)g;
	color.b() = (float)b;

	PointColor pc(x, y, color);

	saveToOutline(pc);
}

void Window::fillOutline()
{
	if (!fillableRenderingMode())
		return;

	filling = true;

	for (int y=0; y<WINDOW_HEIGHT; y++)
	{
		if (outline[y][0].x != -1 && outline[y][1].x != -1)
			plotLine(outline[y][0], outline[y][1]);

		outline[y][0].Reset();
		outline[y][1].Reset();
	}

	filling = false;
}

bool Window::fillableRenderingMode()
{
	if (filling)
		return false;

	return renderMode == GL_TRIANGLES ||
		   renderMode == GL_TRIANGLE_STRIP ||
		   renderMode == GL_TRIANGLE_FAN ||
		   renderMode == GL_POLYGON ||
		   renderMode == GL_QUADS ||
		   renderMode == GL_QUAD_STRIP;
}

#pragma region OPENGL WRAPPERS
/**
 * My OpenGL Wrappers
 */
void Window::cs455_glEnable(GLenum cap)
{
	glEnable(cap);
}

void Window::cs455_glDisable(GLenum cap)
{
	glDisable(cap);
}

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

	switch (renderMode)
	{
		case GL_POINTS:
			pointQ.SetCap(1);
			break;

		case GL_LINES:
			pointQ.SetCap(2);
			break;

		case GL_LINE_STRIP:
			pointQ.SetCap(2);
			break;

		case GL_LINE_LOOP:
		case GL_TRIANGLES:
		case GL_TRIANGLE_STRIP:
			nIsOdd = true;

		case GL_TRIANGLE_FAN:
		case GL_POLYGON:
			pointQ.SetCap(3);
			break;

		case GL_QUADS:
		case GL_QUAD_STRIP:
			pointQ.SetCap(4);
			break;

		case CS455_GL_NONE:
		default: 
			pointQ.SetCap(0);
			break;
	}
}

void Window::cs455_glEnd()
{
	glEnd();

	switch (renderMode)
	{
		case GL_POINTS:	
			break;

		case GL_LINES:
			break;

		case GL_LINE_STRIP:
			break;

		case GL_LINE_LOOP:
			plotLine(pointQ.PopFront(), pointQ.PopFront());
			break;
		
		case GL_TRIANGLES:

			break;

		case GL_TRIANGLE_STRIP:

		case GL_TRIANGLE_FAN:
		case GL_POLYGON:

		case GL_QUADS:
		case GL_QUAD_STRIP:

		case CS455_GL_NONE:
		default: break;
	}

	renderMode = CS455_GL_NONE;
}

void Window::cs455_glVertex2i(GLint x, GLint y)
{
	glVertex2i(x, y);

	PointColor newPc(x, y, currentColor);
	pointQ.Push(newPc);
	
	switch (renderMode)
	{
		case GL_POINTS:	
			setPixel(newPc.x, newPc.y, newPc.color.r(), newPc.color.g(), newPc.color.b());
			break;

		case GL_LINES:
			if (pointQ.Size() == 2)
			{
				PointColor pc = pointQ.PopFront();
				PointColor pc2 = pointQ.PopFront();
				plotLine(pc, pc2);	
			}
			break;

		case GL_LINE_STRIP:
			if (pointQ.Size() == 2)
			{
				plotLine(pointQ.PopBack(), pointQ.Front());
			}
			break;

		case GL_LINE_LOOP:
			if (pointQ.Size() == 2)
			{
				plotLine(pointQ.Back(), pointQ.Front());
			}
			else if (pointQ.Size() == 3)
			{
				PointColor pc0 = pointQ.PopFront();
				PointColor pc1 = pointQ.PopFront();
				plotLine(pc1, pc0);
				pointQ.Push(pc0);
			}
			break;

		case GL_TRIANGLES:
			if (pointQ.Size() == 3)
			{
				PointColor pc0, pc1, pc2;
				pc0 = pointQ.PopFront();
				pc1 = pointQ.PopFront();
				pc2 = pointQ.PopFront();

				plotLine(pc0, pc1);
				plotLine(pc1, pc2);
				plotLine(pc2, pc0);

				// fill outline
				fillOutline();
			}
			break;

		case GL_TRIANGLE_STRIP:
			if (pointQ.Size() == 3)
			{
				PointColor pc0 = pointQ.PopBack();
				if (nIsOdd)
				{
					plotLine(pc0, pointQ.Back());
					plotLine(pointQ.Back(), pointQ.Front());
					plotLine(pointQ.Front(), pc0);
					nIsOdd = false;
				}
				else
				{
					plotLine(pointQ.Back(), pc0);
					plotLine(pc0, pointQ.Front());
					plotLine(pointQ.Front(), pointQ.Back());
					nIsOdd = true;
				}
				fillOutline();
			}
			break;

		case GL_TRIANGLE_FAN:
		case GL_POLYGON:
			if (pointQ.Size() == 3)
			{
				PointColor pc1, pc2;
				pc1 = pointQ.PopFront();
				pc2 = pointQ.PopFront();

				plotLine(pointQ.Back(), pc1);
				plotLine(pc1, pc2);
				plotLine(pc2, pointQ.Back());
				fillOutline();

				pointQ.Push(pc1);
			}
			break;

		case GL_QUADS:
			if (pointQ.Size() == 4)
			{
				PointColor pc0, pc1, pc2, pc3;
				pc0 = pointQ.PopFront();
				pc1 = pointQ.PopFront();
				pc2 = pointQ.PopFront();
				pc3 = pointQ.PopFront();

				plotLine(pc0, pc1);
				plotLine(pc1, pc2);
				plotLine(pc2, pc0);
				fillOutline();

				plotLine(pc0, pc2);
				plotLine(pc2, pc3);
				plotLine(pc3, pc0);
				fillOutline();
			}
			break;

		case GL_QUAD_STRIP:
			if (pointQ.Size() == 4)
			{
				PointColor pc0, pc1;
				pc0 = pointQ.PopBack();
				pc1 = pointQ.PopBack();

				plotLine(pc0, pc1);
				plotLine(pc1, pointQ.Back());
				plotLine(pointQ.Back(), pc0);
				fillOutline();

				plotLine(pc1, pointQ.Back());
				plotLine(pointQ.Back(), pointQ.Front());
				plotLine(pointQ.Front(), pc1);
				fillOutline();
			}
			break;

		case CS455_GL_NONE:
		default: break;
	}

	return;
}

void Window::cs455_glVertex2f(GLfloat x, GLfloat y)
{
	glVertex2f(x, y);
}

void Window::cs455_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	glVertex3f(x, y, z);
}

void Window::cs455_glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glVertex4f(x, y, z, w);
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

void Window::cs455_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
}

void Window::cs455_glMatrixMode(GLenum mode)
{
	if (mode < 0 || mode >= MATRIX_MODE_COUNT)
		return;

	currentMatrix = mode;
}

void Window::cs455_glLoadIdentity(void)
{
	glLoadIdentity();

	activeMatrix[currentMatrix].setIdentity();
}

void Window::cs455_glPushMatrix(void)
{
	glPushMatrix();

	matrixStack[currentMatrix].push(activeMatrix[currentMatrix]);
}

void Window::cs455_glPopMatrix(void)
{
	glPopMatrix();

	activeMatrix[currentMatrix] = matrixStack[currentMatrix].top();
	matrixStack[currentMatrix].pop();
}

void Window::cs455_glLoadMatrixd(const GLdouble *m)
{
	glLoadMatrixd(m);

	activeMatrix[currentMatrix] = Map<Matrix455>(m, 4, 4);
}

void Window::cs455_glMultMatrixd(const GLdouble *m)
{
	glMultMatrixd(m);
}

void Window::cs455_glRotatef(GLfloat theta, GLfloat x, GLfloat y, GLfloat z)
{
	glRotatef(theta, x, y, z);
}

void Window::cs455_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	glTranslatef(x, y, z);
}

void Window::cs455_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	glScalef(x, y, z);
}

void Window::cs455_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
{
	glOrtho(left, right, bottom, top, near, far);
}

#pragma endregion
