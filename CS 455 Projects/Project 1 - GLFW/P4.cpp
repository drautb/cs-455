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
	else if (sceneToRender == 2)
		renderGouraud(w);
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

void P4::renderGouraud(Window *w)
{
	w->cs455_glViewport(0, 0, 640, 480);
	w->cs455_glMatrixMode(Window::CS455_GL_PROJECTION);
	w->cs455_glLoadIdentity();
	w->cs455_glMatrixMode(Window::CS455_GL_MODELVIEW);
	w->cs455_glLoadIdentity();

	w->cs455_glEnable(GL_NORMALIZE);
	w->cs455_glEnable(GL_LIGHTING);
	w->cs455_glEnable(GL_COLOR_MATERIAL);
	w->cs455_glEnable(GL_LIGHT0);

	float diffuse_color[4] = {1.0f,1.0f,1.0f,1.0f};
	float ambient_color[4] = {0.1f,0.1f,0.1f,1.0f};
	float position[4] = {0.0f,3.0f,-20.0f,1.0f};

	w->cs455_glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
	w->cs455_glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	w->cs455_glLightfv(GL_LIGHT0, GL_POSITION, position);

	w->cs455_glColor3f(1,0,0);
	float dp = (float)(M_PI/16); // 16 picked arbitrarily; try other numbers too
	
	w->cs455_glBegin(GL_TRIANGLES);
	
	for(float theta = 0; theta < 2*M_PI; theta+=dp)
	{
		for(float phi = 0; phi < M_PI; phi+=dp)
		{
			w->cs455_glNormal3f(cos(theta)*sin(phi), cos(phi), sin(theta)*sin(phi));
			w->cs455_glVertex3f(cos(theta)*sin(phi), cos(phi), sin(theta)*sin(phi));
			w->cs455_glNormal3f(cos(theta+dp)*sin(phi), cos(phi), sin(theta+dp)*sin(phi));
			w->cs455_glVertex3f(cos(theta+dp)*sin(phi), cos(phi), sin(theta+dp)*sin(phi));
			w->cs455_glNormal3f(cos(theta+dp)*sin(phi+dp), cos(phi+dp), sin(theta+dp)*sin(phi+dp));
			w->cs455_glVertex3f(cos(theta+dp)*sin(phi+dp), cos(phi+dp), sin(theta+dp)*sin(phi+dp));
			w->cs455_glNormal3f(cos(theta)*sin(phi), cos(phi), sin(theta)*sin(phi));
			w->cs455_glVertex3f(cos(theta)*sin(phi), cos(phi), sin(theta)*sin(phi));
			w->cs455_glNormal3f(cos(theta+dp)*sin(phi+dp), cos(phi+dp), sin(theta+dp)*sin(phi+dp));
			w->cs455_glVertex3f(cos(theta+dp)*sin(phi+dp), cos(phi+dp), sin(theta+dp)*sin(phi+dp));
			w->cs455_glNormal3f(cos(theta)*sin(phi+dp), cos(phi+dp), sin(theta)*sin(phi+dp));
			w->cs455_glVertex3f(cos(theta)*sin(phi+dp), cos(phi+dp), sin(theta)*sin(phi+dp));
		}
	}

	w->cs455_glEnd();

	w->cs455_glDisable(GL_NORMALIZE);
	w->cs455_glDisable(GL_LIGHTING);
	w->cs455_glDisable(GL_COLOR_MATERIAL);
	w->cs455_glDisable(GL_LIGHT0);

	return;
}