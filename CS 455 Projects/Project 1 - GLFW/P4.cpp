#include "P4.h"
#include "Window.h"

void P4::redraw(Window *w)
{
	int sceneToRender = w->SceneToRender();
	if (sceneToRender < 0) sceneToRender *= -1;
	sceneToRender %= SCENE_COUNT;

	if (sceneToRender == 0)
		renderFrustum(w);
	else if (sceneToRender == 1)
		renderPerspective(w);
}

void P4::renderFrustum(Window *w)
{
	w->cs455_glMatrixMode(Window::CS455_GL_PROJECTION);
	w->cs455_glLoadIdentity();
	w->cs455_glFrustum(-0.1, 0.1, -0.1*480/640, 0.1*480/640, 0.1, 10);

	w->cs455_glMatrixMode(Window::CS455_GL_MODELVIEW);
	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.0f,0.0f,1.0f);
		w->cs455_glVertex3f(-0.4f,-0.6f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.6f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(-0.4f,-0.6f,-1.0f);
		w->cs455_glColor3f(1.0f,0.0f,1.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(-0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
	w->cs455_glEnd();
}

void P4::renderPerspective(Window *w)
{
	w->cs455_glMatrixMode(Window::CS455_GL_PROJECTION);
	w->cs455_glLoadIdentity();
	w->cs455_gluPerspective(90, double(640)/480, .1, 10);

	w->cs455_glMatrixMode(Window::CS455_GL_MODELVIEW);
	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.0f,0.0f,1.0f);
		w->cs455_glVertex3f(-0.4f,-0.6f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.6f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(-0.4f,-0.6f,-1.0f);
		w->cs455_glColor3f(1.0f,0.0f,1.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.4f,-0.1f,-1.0f);
		w->cs455_glVertex3f(0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(-0.3f,0.0f,-2.0f);
		w->cs455_glVertex3f(-0.4f,-0.1f,-1.0f);
	w->cs455_glEnd();
}