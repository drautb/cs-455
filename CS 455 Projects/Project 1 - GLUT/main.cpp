#include <windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

int main(int, char**);
static void init(void);
static void redraw(void);
static void handleKeyboard(unsigned char key, int x, int y);

// CONSTANTS
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int DIMENSIONS = 3;

const int MODE_OPENGL = 1;
const int MODE_MINE = 2;

// Globals
float raster[WINDOW_WIDTH * WINDOW_HEIGHT * DIMENSIONS];
int drawMode;

bool keys[256];

// Main loop
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS 455");

	glutDisplayFunc(redraw);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	init();

	glutMainLoop();

	return 0;
}

static void init(void)
{
	for (int i=0; i<WINDOW_WIDTH*WINDOW_HEIGHT*DIMENSIONS; i++)
		raster[i] = ((float)i)/(WINDOW_WIDTH * WINDOW_HEIGHT * DIMENSIONS);
}

static void redraw(void)
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

	glutSwapBuffers();      //swaps the front and back buffers
}