#include <iostream>

#include <cmath>

#include <GL/glfw.h>

#include "Window.h"
#include "cs455Utils.h"
#include "P2.h"
#include "P3.h"
#include "P4.h"

using namespace Eigen;

void GLFWCALL ResizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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
		activeMatrix[s] = Matrix455::Identity();
		matrixStack[s].push(activeMatrix[s]);
	}

	composedMatrix = Matrix455::Identity();

	vpXMin = vpYMin = 0;
	vpWidth = WINDOW_WIDTH;
	vpHeight = WINDOW_HEIGHT;
	zNear = -1.0f;
	zFar = 1.0f;

	transformedPt.Zero();

	glCapEnabled = 0;

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

	//cs455_glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1); // only for project 2; delete thereafter!

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
	//P3::redraw(this);
	P4::redraw(this);

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
			setPixel(x, y, 0.0f, 1, 1, 1);
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

void Window::renderPoint()
{
	PointColor newPc((int)transformedPt.x(), (int)transformedPt.y(), transformedPt.z(), currentColor);
	pointQ.Push(newPc);

	switch (renderMode)
	{
		case GL_POINTS:	
			setPixel(newPc.x, newPc.y, newPc.z, newPc.color.r(), newPc.color.g(), newPc.color.b());
			break;

		case GL_LINES:
			if (pointQ.Size() == 2)
			{
				PointColor pc = pointQ.PopFront();
				PointColor pc2 = pointQ.PopFront();
				setZPlane(pc, pc2, pc2);
				plotLine(pc, pc2);	
			}
			break;

		case GL_LINE_STRIP:
			if (pointQ.Size() == 2)
			{
				PointColor pc = pointQ.PopBack();
				PointColor pc2 = pointQ.Front();
				setZPlane(pc, pc2, pc2);
				plotLine(pc, pc2);
			}
			break;

		case GL_LINE_LOOP:
			if (pointQ.Size() == 2)
			{
				setZPlane(pointQ.Back(), pointQ.Front(), pointQ.Front());
				plotLine(pointQ.Back(), pointQ.Front());
			}
			else if (pointQ.Size() == 3)
			{
				PointColor pc0 = pointQ.PopFront();
				PointColor pc1 = pointQ.PopFront();
				setZPlane(pc1, pc0, pc0);
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

				setZPlane(pc0, pc1, pc2);

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
					setZPlane(pc0, pointQ.Back(), pointQ.Front());
					plotLine(pc0, pointQ.Back());
					plotLine(pointQ.Back(), pointQ.Front());
					plotLine(pointQ.Front(), pc0);
					nIsOdd = false;
				}
				else
				{
					setZPlane(pointQ.Back(), pc0, pointQ.Front());
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

				setZPlane(pointQ.Back(), pc1, pc2);

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

				setZPlane(pc0, pc1, pc2);

				plotLine(pc0, pc1);
				plotLine(pc1, pc2);
				plotLine(pc2, pc0);
				fillOutline();

				setZPlane(pc0, pc2, pc3);

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

				setZPlane(pc0, pc1, pointQ.Back());

				plotLine(pc0, pc1);
				plotLine(pc1, pointQ.Back());
				plotLine(pointQ.Back(), pc0);
				fillOutline();

				setZPlane(pc1, pointQ.Back(), pointQ.Front());

				plotLine(pc1, pointQ.Back());
				plotLine(pointQ.Back(), pointQ.Front());
				plotLine(pointQ.Front(), pc1);
				fillOutline();
			}
			break;

		case CS455_GL_NONE:
		default: break;
	}
}

void Window::setPixel(unsigned int x, unsigned int y, float z, double r, double g, double b)
{
	if (x < vpXMin + vpWidth && x > vpXMin &&
		y < vpYMin + vpHeight && y > vpYMin)
	{
		if ((glCapEnabled & GL_DEPTH_TEST) != 0)
		{
			int zBufIdx = y * WINDOW_WIDTH + x;
			if (z >= zNear && z <= zFar && zBuffer[zBufIdx] > z)
			{
				zBuffer[zBufIdx] = z;
				raster[((y*WINDOW_WIDTH) + x)*3 + 0] = (float)r;
				raster[((y*WINDOW_WIDTH) + x)*3 + 1] = (float)g;
				raster[((y*WINDOW_WIDTH) + x)*3 + 2] = (float)b;
			}
		}
		else
		{
			raster[((y*WINDOW_WIDTH) + x)*3 + 0] = (float)r;
			raster[((y*WINDOW_WIDTH) + x)*3 + 1] = (float)g;
			raster[((y*WINDOW_WIDTH) + x)*3 + 2] = (float)b;
		}
	}

	/*
	if (x > WINDOW_WIDTH || y > WINDOW_HEIGHT)
		return;

	raster[((y*WINDOW_WIDTH) + x)*3 + 0] = (float)r;
	raster[((y*WINDOW_WIDTH) + x)*3 + 1] = (float)g;
	raster[((y*WINDOW_WIDTH) + x)*3 + 2] = (float)b;
	*/

	return;
}

void Window::setZPlane(PointColor& pc0, PointColor& pc1, PointColor& pc2)
{
	if ((glCapEnabled & GL_DEPTH_TEST) == 0)
		return;

	p0.x() = (float)pc0.x;
	p0.y() = (float)pc0.y;
	p0.z() = pc0.z;

	p1.x() = (float)pc1.x;
	p1.y() = (float)pc1.y;
	p1.z() = pc1.z;

	p2.x() = (float)pc2.x;
	p2.y() = (float)pc2.y;
	p2.z() = pc2.z;

	currentPlane = currentPlane.Through(p0, p1, p2);
	coeffs = currentPlane.coeffs();
	a = coeffs.x();
	b = coeffs.y();
	c = coeffs.z();
	d = coeffs.w();

	return;
}

float Window::getZValue(int x, int y)
{
	if ((glCapEnabled & GL_DEPTH_TEST) == 0)
		return 0.0f;

	return (float)((-a * x - b * y - d) / c);
}

void Window::plotLine(int x0, int y0, float z0, int x1, int y1, float z1, double r0, double g0, double b0, double r1, double g1, double b1)
{
	//cout << endl << "Plotting line from " << x0 << ", " << y0 << " to " << x1 << ", " << y1;
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
				saveToOutline(x0, y0, getZValue(x0, y0), red, green, blue);
				if (lineWidth > 1.0f)
				{
					for (int x=x0-(int)(lineWidth/2.0f); x<x0+(int)(lineWidth/2); x++)
						setPixel(x, y0, getZValue(x, y0), red, green, blue);
					y0--;
				}
				else
				{
					setPixel(x0, y0, getZValue(x0, y0), red, green, blue);
					y0--;
				}
				red += dRed;
				green += dGreen;
				blue += dBlue;
			}
		}
		else
		{
			while (y0 < y1)
			{
				saveToOutline(x0, y0, getZValue(x0, y0), red, green, blue);
				if (lineWidth > 1.0f)
				{
					for (int x=x0-(int)(lineWidth/2.0f); x<x0+(int)(lineWidth/2); x++)
						setPixel(x, y0, getZValue(x, y0), red, green, blue);
					y0++;
				}
				else
				{
					setPixel(x0, y0, getZValue(x0, y0), red, green, blue);
					y0++;
				}
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
						setPixel(x0, y, getZValue(x0, y), red, green, blue);
					x0--;
				}
				else
				{
					setPixel(x0, y0, getZValue(x0, y0), red, green, blue);
					x0--;
				}
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
						setPixel(x0, y, getZValue(x0, y), red, green, blue);
					x0++;
				}
				else
				{
					setPixel(x0, y0, getZValue(x0, y0), red, green, blue);
					x0++;
				}
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
	float startZ = 0.0f;

	if (x0 < x1)
	{
		startX = x0;
		startY = y0;
		startZ = z0;
		endX = x1;
		endY = y1;
	}
	else
	{
		startX = x1;
		startY = y1;
		startZ = z1;
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

	saveToOutline(startX, startY, startZ, red, green, blue);
	setPixel(startX, startY, getZValue(startX, startY), red, green, blue);
	for (int step=1; step<=deltaStep; step++)
	{
		if (p0 < 0)
		{
			if (dx > dy)
				startX += stepX;
			else
			{
				startY += stepY;
				saveToOutline(startX, startY, getZValue(startX, startY), red, green, blue);
			}

			p0 += d2;
		}
		else
		{
			startX += stepX;
			startY += stepY;
			saveToOutline(startX, startY, getZValue(startX, startY), red, green, blue);

			p0 += d2xy;
		}

		if (lineWidth > 1.0f)
		{
			if (dx > dy) // change in x is greater, so the line should be thicker in the y
			{
				for (int y=startY-(int)(lineWidth/2.0f); y<startY+(int)(lineWidth/2); y++)
					setPixel(startX, y, getZValue(startX, y), red, green, blue);
			} 
			else // change in y is greater, so the line should be thicker in the x
			{
				for (int x=startX-(int)(lineWidth/2.0f); x<startX+(int)(lineWidth/2); x++)
					setPixel(x, startY, getZValue(x, startY), red, green, blue);
			}
		}
		else
			setPixel(startX, startY, getZValue(startX, startY), red, green, blue);

		red += dRed;
		green += dGreen;
		blue += dBlue;
	}

	return;
}

void Window::plotLine(int x0, int y0, float z0, int x1, int y1, float z1, Vector455& startColor, Vector455& endColor)
{
	plotLine(x0, y0, z0, x1, y1, z1, startColor.r(), startColor.g(), startColor.b(), endColor.r(), endColor.g(), endColor.b());
}

void Window::plotLine(PointColor& pc0, PointColor& pc1)
{
	plotLine(pc0.x, pc0.y, pc0.z, pc1.x, pc1.y, pc1.z, pc0.color, pc1.color);
}

void Window::plotLine(int x0, int y0, float z0, int x1, int y1, float z1)
{
	plotLine(x0, y0, z0, x1, y1, z1, currentColor, currentColor);
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

	if (pc.y < 0 || pc.y > WINDOW_HEIGHT)
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

void Window::saveToOutline(int x, int y, float z, double r, double g, double b)
{
	if (!fillableRenderingMode())
		return;

	Vector455 color;
	color.r() = (float)r;
	color.g() = (float)g;
	color.b() = (float)b;

	PointColor pc(x, y, z, color);

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
	}

	clearOutline();

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

void Window::loadDataIntoMatrix(Matrix455 *mat, const GLdouble *data)
{
	for (int row=0; row<Matrix455::ROWS; row++)
	{
		for (int col=0; col<Matrix455::COLS; col++)
		{
			(*mat)(row, col) = (float)(data[col*Matrix455::COLS + row]);
		}
	}

	return;
}

void Window::transformPoint(double x, double y, double z, double w)
{
	transformedPt = Vector455::Identity();
	transformedPt.x() = (float)x;
	transformedPt.y() = (float)y;
	transformedPt.z() = (float)z;
	transformedPt.w() = (float)w;

	// 1) Apply projection and modelview transformations
	transformedPt = composedMatrix * transformedPt;

	// 2) Divide by w
	transformedPt /= transformedPt.w();

	// 3) Size to viewport
	transformedPt.x() = (float)((transformedPt.x() + 1) * vpWidth / 2 + vpXMin);
	transformedPt.y() = (float)((transformedPt.y() + 1) * vpHeight / 2 + vpYMin);

	return;
}

#pragma region OPENGL WRAPPERS
/**
 * My OpenGL Wrappers
 */
void Window::cs455_glEnable(GLenum cap)
{
	glEnable(cap);

	glCapEnabled |= cap;
}

void Window::cs455_glDisable(GLenum cap)
{
	glDisable(cap);

	glCapEnabled &= ~cap;
}

bool Window::cs455_glIsEnabled(GLenum cap)
{
	return (glIsEnabled(cap) && ((glCapEnabled & cap) != 0));
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
				setPixel(x, y, 0.0f, clearR, clearG, clearB);
		}
	}

	if ((mask & GL_DEPTH_BUFFER_BIT) != 0)
	{
		for (int x=0; x<WINDOW_WIDTH; x++)
		{
			for (int y=0; y<WINDOW_HEIGHT; y++)
				zBuffer[y*WINDOW_WIDTH + x] = 1.0f;
		}
	}

	waterMarkMine();

	return;
}

void Window::cs455_glBegin(GLenum mode)
{
	glBegin(mode);

	// Generate the composed matrix
	composedMatrix = activeMatrix[0] * activeMatrix[1];

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

	transformPoint(x, y);
	
	renderPoint();

	return;
}

void Window::cs455_glVertex2f(GLfloat x, GLfloat y)
{
	glVertex2f(x, y);

	transformPoint(x, y);
	
	renderPoint();

	return;
}

void Window::cs455_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	glVertex3f(x, y, z);

	transformPoint(x, y, z);

	renderPoint();

	return;
}

void Window::cs455_glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glVertex4f(x, y, z, w);

	transformPoint(x, y, z, w);

	renderPoint();

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

void Window::cs455_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);

	if (x < 0 || y < 0 || (width - x) > WINDOW_WIDTH || (height - y) > WINDOW_HEIGHT)
	{
		cout << endl << "Invalid Viewport Specified";
		return;
	}

	vpXMin = x;
	vpYMin = y;
	vpWidth = width;
	vpHeight = height;

	return;
}

void Window::cs455_glMatrixMode(GLenum mode)
{
	if (mode == CS455_GL_MODELVIEW)
		glMatrixMode(GL_MODELVIEW);
	else if (mode == CS455_GL_PROJECTION)
		glMatrixMode(GL_PROJECTION);

	if (mode < 0 || mode >= MATRIX_MODE_COUNT)
		return;

	currentMatrix = mode;
}

void Window::cs455_glLoadIdentity(void)
{
	glLoadIdentity();

	activeMatrix[currentMatrix] = Matrix455::Identity();
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

	loadDataIntoMatrix(&activeMatrix[currentMatrix], m);
}

void Window::cs455_glMultMatrixd(const GLdouble *m)
{
	glMultMatrixd(m);

	loadDataIntoMatrix(&temp, m);

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_glRotatef(GLfloat theta, GLfloat x, GLfloat y, GLfloat z)
{
	glRotatef(theta, x, y, z);

	temp = Matrix455::Identity();
	
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

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	glTranslatef(x, y, z);

	temp = Matrix455::Identity();

	temp(0, 3) = x;
	temp(1, 3) = y;
	temp(2, 3) = z;

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	glScalef(x, y, z);

	temp = Matrix455::Identity();

	temp(0, 0) = x;
	temp(1, 1) = y;
	temp(2, 2) = z;

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_glScaleFixed(GLdouble sx, GLdouble sy, GLdouble sz, GLdouble cx, GLdouble cy, GLdouble cz)
{
	GLdouble data[16] = { 1.0, 0.0, 0.0, 0.0, 
						  0.0, 1.0, 0.0, 0.0,
						  0.0, 0.0, 1.0, 0.0,
						  0.0, 0.0, 0.0, 1.0 };

	data[12] = cx;
	data[13] = cy;
	data[14] = cz;
	cs455_glMultMatrixd(data);
	
	cs455_glScalef((float)sx, (float)sy, (float)sz);

	data[12] = -cx;
	data[13] = -cy;
	data[14] = -cz;
	cs455_glMultMatrixd(data);

	return;
}

void Window::cs455_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
{
	glOrtho(left, right, bottom, top, near, far);

	temp = Matrix455::Identity();

	temp(0, 0) = (float)(2 / (right - left));
	temp(0, 3) = (float)-((right + left) / (right - left));

	temp(1, 1) = (float)(2 / (top - bottom));
	temp(1, 3) = (float)-((top + bottom) / (top - bottom));

	temp(2, 2) = (float)(-2 / (far - near));
	temp(2, 3) = (float)-((far + near) / (far - near));

	zNear = (float)near;
	zFar = (float)far;

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glFrustum(left, right, bottom, top, zNear, zFar);

	temp = Matrix455::Zero();
	temp(0, 0) = (float)((2 * zNear) / (right - left));
	temp(0, 2) = (float)((right + left) / (right - left));

	temp(1, 1) = (float)((2 * zNear) / (top - bottom));
	temp(1, 2) = (float)((top + bottom) / (top - bottom));

	temp(2, 2) = (float)(-(zFar + zNear) / (zFar - zNear));
	temp(2, 3) = (float)(-(2 * zFar * zNear) / (zFar - zNear));

	temp(3, 2) = -1.0f;

	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_gluLookAt(GLdouble ex, GLdouble ey, GLdouble ez, GLdouble cx, GLdouble cy, GLdouble cz, GLdouble ux, GLdouble uy, GLdouble uz)
{
	gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
	
	f.x() = (float)(cx - ex);
	f.y() = (float)(cy - ey);
	f.z() = (float)(cz - ez);

	up.x() = (float)(ux);
	up.y() = (float)(uy);
	up.z() = (float)(uz);

	f.normalize();
	up.normalize();

	s = f.cross(up);
	s.normalize();

	u = s.cross(f);
	u.normalize();

	temp = Matrix455::Identity();

	temp(0, 0) = s.x();
	temp(0, 1) = s.y();
	temp(0, 2) = s.z();

	temp(1, 0) = u.x();
	temp(1, 1) = u.y();
	temp(1, 2) = u.z();

	temp(2, 0) = -f.x();
	temp(2, 1) = -f.y();
	temp(2, 2) = -f.z();

	activeMatrix[currentMatrix] *= temp;
	
	temp = Matrix455::Identity();
	temp(0, 3) = -(float)(ex);
	temp(1, 3) = -(float)(ey);
	temp(2, 3) = -(float)(ez);
	
	activeMatrix[currentMatrix] *= temp;
}

void Window::cs455_gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	gluPerspective(fovy, aspect, zNear, zFar);

	fovy *= (M_PI / 180.0);
	double f = 1.0 / tan(fovy / 2.0);

	temp = Matrix455::Zero();
	temp(0, 0) = (float)(f / aspect);

	temp(1, 1) = (float)(f);

	temp(2, 2) = (float)((zFar + zNear) / (zNear - zFar));
	temp(2, 3) = (float)((2 * zFar * zNear) / (zNear - zFar));

	temp(3, 2) = -1.0f;

	activeMatrix[currentMatrix] *= temp;
}

#pragma endregion
