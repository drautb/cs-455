#include <iostream>

#include <GL/glfw.h>

#include <Eigen\Core>

#include "Matrix455.h"
#include "Vector455.h"

using namespace std;
using namespace Eigen;

// Constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int DIMENSIONS = 3;
const int RASTER_SIZE = WINDOW_WIDTH * WINDOW_HEIGHT * DIMENSIONS;

const int MODE_OPENGL = 1;
const int MODE_MINE = 2;

// Globals
int keys[256];
int drawMode = 1;
float raster[RASTER_SIZE];

// Function Declarations
int main(int argc, char *argv[]);
void init(void);
void redraw(void);
void keyboardCallback(int key, int keyState);
void testProject1(void);

// Main loop
int main(int argc, char *argv[])
{
	int running = GL_TRUE;

	// Initialize GLFW
	if(!glfwInit())
		return -1;

	// Open an OpenGL window
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
	{
		glfwTerminate();
		return -1;
	}
	
	// Keyboard callback
	glfwSetKeyCallback(keyboardCallback);

	init();

	testProject1();

	// Main loop
	while(running)
	{
		// Handle input
		if (keys['1'] == GLFW_PRESS)
			drawMode = MODE_OPENGL;
		if (keys['2'] == GLFW_PRESS)
			drawMode = MODE_MINE;

		// Redraw everything
		redraw();

		// Check if ESC key was pressed or window was closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	return 0;
}

void init(void)
{
	drawMode = MODE_OPENGL;

	for (int i=0; i<RASTER_SIZE; i++)
		raster[i] = ((float)i)/(RASTER_SIZE);
}

void redraw(void)
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (drawMode == MODE_MINE)
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

void keyboardCallback(int key, int keyState)
{
	keys[key] = keyState;
}

void testProject1()
{
	cout << "TESTING PROJECT 1 MATH COMPONENTS" << endl;

	Matrix455 mat1;
	mat1 = Matrix455::Zero();
	mat1(0, 0) = 1.0f;
	mat1(0, 1) = 2.0f;
	mat1(0, 2) = 3.0f;
	mat1(0, 3) = 4.0f;
	mat1(1, 1) = 5.0f;
	mat1(1, 2) = 6.0f;
	mat1(1, 3) = 7.0f;
	mat1(2, 2) = 8.0f;
	mat1(2, 3) = 9.0f;
	cout << endl << "Mat1: " << endl << mat1 << endl;

	Matrix455 mat2;
	mat2 = Matrix455::Zero();
	float vals[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 2.0f, 0.0f, 4.0f };
	mat2 = vals;
	cout << endl << "Mat2: " << endl << mat2 << endl;

	cout << endl << "Mat1 * Mat2: " << endl << mat1*mat2 << endl;

	cout << endl << "Accessing the array of data for mat2... " << endl;
	const float *data = mat2.toArray();
	for (int i=0; i<16; i++)
		cout << "\t[" << i << "]: " << data[i];

	cout << endl << endl << "Creating vector..." << endl;

	Vector455 vec1;
	vec1 = Vector455::Zero();
	float vecVals[4] = { 2.0f, 3.0f, 5.0f, 7.0f };
	vec1 = vecVals;
	cout << endl << "Vector1: " << endl << vec1 << endl;

	cout << endl << "Multiplying mat1 * vec1 = " << endl << mat1 * vec1 << endl;

	cout << endl << "Done." << endl;
}