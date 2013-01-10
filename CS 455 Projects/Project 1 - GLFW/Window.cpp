#include <GL/glfw.h>

#include "Window.h"
#include "cs455Utils.h"

Window::Window(void)
{
	glfwInit();

	drawMode = MODE_OPENGL;

	for (int i=0; i<RASTER_SIZE; i++)
		raster[i] = ((float)i)/(RASTER_SIZE);
}

Window::~Window(void)
{
	// Close window and terminate GLFW
	glfwTerminate();
}

bool Window::Open(void)
{
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
		return false;

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
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (drawMode == MODE_CS_455)
	{
		GLint oldMatrixMode;
		GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_TEST);
		glGetIntegerv(GL_MATRIX_MODE, &oldMatrixMode);
		
		glMatrixMode(GL_MODELVIEW); 
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glRasterPos2f(-1, -1);
		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, raster);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(oldMatrixMode);

		if (depthWasEnabled)
			glEnable(GL_DEPTH_TEST);
	}

	glFlush();

	glfwSwapBuffers();
}

void Window::checkRenderingMode(void)
{
	if (KEY_DOWN('1'))
		drawMode = MODE_OPENGL;
	else if (KEY_DOWN('2'))
		drawMode = MODE_CS_455;

	return;
}