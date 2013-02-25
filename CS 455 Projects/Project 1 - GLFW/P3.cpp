#include "P3.h"
#include "Window.h"

void P3::redraw(Window *w)
{
	int sceneToRender = w->SceneToRender();
	if (sceneToRender < 0) sceneToRender *= -1;
	sceneToRender %= SCENE_COUNT;

	if (sceneToRender == 0)
		renderViewport(w);
	else if (sceneToRender == 1)
		render1OverW(w);
	else if (sceneToRender == 2)
		renderDepthBuffer(w);
	else if (sceneToRender == 3)
		renderEasyPointClipping(w);
	else if (sceneToRender == 4)
		renderMatrixManips(w);
	else if (sceneToRender == 5)
		renderMatrixStacks(w);
	else if (sceneToRender == 6)
		renderRotate(w);
	else if (sceneToRender == 7)
		renderTranslate(w);
	else if (sceneToRender == 8)
		renderScale(w);
	else if (sceneToRender == 9)
		renderOrtho(w);
}

void P3::renderViewport(Window *w)
{
	w->cs455_glViewport(0,0,320,240);
	
	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.0f,1.0f,0.0f);
		w->cs455_glVertex3f(-1.0f,0.0f,0.0f);
		w->cs455_glVertex3f(0.0f,-0.8f,0.0f);
		w->cs455_glVertex3f(0.5f,0.8f,0.0f);
	w->cs455_glEnd();
	
	w->cs455_glViewport(320,240,320,240);
	
	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.0f,0.0f,1.0f);
		w->cs455_glVertex3f(-1.0f,0.8f,0.0f);
		w->cs455_glVertex3f(0.1f,-0.8f,0.0f);
		w->cs455_glVertex3f(0.5f,0.8f,0.0f);
	w->cs455_glEnd();

	//Restore your viewport to the whole screen
	w->cs455_glViewport(0,0,640,480);
}

void P3::render1OverW(Window *w)
{
	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(1.0f,0.0f,0.0f);
		w->cs455_glVertex4f(-1.0f,0.2f,0.0f,1.0f);
		w->cs455_glVertex4f(0.0f,0.8f,0,1.0f);
		w->cs455_glVertex4f(1.0f,0.2f,0.0f,1.0f);
		w->cs455_glColor3f(1.0f,0.0f,1.0f);
		w->cs455_glVertex4f(-1.0f,-0.8f,0.0f,2.0f);
		w->cs455_glVertex4f(0.0f,-0.2f,0,2.0f);
		w->cs455_glVertex4f(1.0f,-0.8f,0.0f,2.0f);
	w->cs455_glEnd();
}

void P3::renderDepthBuffer(Window *w)
{
	w->cs455_glEnable(GL_DEPTH_TEST);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.0f,1.0f,1.0f);
		w->cs455_glVertex3f(-0.5f,0.2f,0.5f);
		w->cs455_glVertex3f(0.0f,-0.5f,0.0f);
		w->cs455_glVertex3f(0.5f,0.2f,-0.5f);
		w->cs455_glColor3f(1.0f,1.0f,0.0f);
		w->cs455_glVertex3f(-0.5f,-0.2f,-0.5f);
		w->cs455_glVertex3f(0.0f,0.5f,0.0f);
		w->cs455_glVertex3f(0.5f,-0.2f,0.5f);
	w->cs455_glEnd();

	w->cs455_glDisable(GL_DEPTH_TEST);
}

void P3::renderEasyPointClipping(Window *w)
{
	w->cs455_glEnable(GL_DEPTH_TEST);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.5f,1.0f,1.0f);
		w->cs455_glVertex3f(0.5f,0.0f,0.0f);
		w->cs455_glVertex3f(0.0f,0.5f,-2.0f);
		w->cs455_glVertex3f(0.0f,-0.5f,2.0f);
	w->cs455_glEnd();

	w->cs455_glViewport(50,50,200,400);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(1.0f,1.0f,0.5f);
		w->cs455_glVertex3f(-1.4f,-1.2f,-0.5f);
		w->cs455_glVertex3f(0.0f,1.2f,0.0f);
		w->cs455_glVertex3f(1.5f,-0.2f,0.5f);
	w->cs455_glEnd();

	//Restore your viewport to the whole screen
	w->cs455_glViewport(0,0,640,480);

	w->cs455_glDisable(GL_DEPTH_TEST);
}

void P3::renderMatrixManips(Window *w)
{
	double translate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, -1.2,0.3,0,1};
	double rotate[16] = {cos(M_PI/2),sin(M_PI/2),0,0, -sin(M_PI/2),cos(M_PI/2),0,0, 0,0,1,0, 0,0,0,1};

	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.5f,0.2f,1.0f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadMatrixd(translate);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.5f,0.8f,0.2f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.2f,0.6f,1.0f);
		w->cs455_glVertex3f(0.5f,-0.4f,0.0f);
		w->cs455_glVertex3f(0.8f,-0.4f,0.0f);
		w->cs455_glVertex3f(0.65f,-0.7f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadMatrixd(rotate);

	w->cs455_glMultMatrixd(translate);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.9f,0.2f,0.4f);
		w->cs455_glVertex3f(0.5f,-0.4f,0.0f);
		w->cs455_glVertex3f(0.8f,-0.4f,0.0f);
		w->cs455_glVertex3f(0.65f,-0.7f,0.0f);
	w->cs455_glEnd(); 

	w->cs455_glLoadIdentity();
}

void P3::renderMatrixStacks(Window *w)
{
	tree(w, 8);
}

void P3::renderRotate(Window *w)
{
	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.5f,0.2f,1.0f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glRotatef(90.0f,0.0f,0.0f,1.0f);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.1f,0.2f,1.0f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadIdentity();
}

void P3::renderTranslate(Window *w)
{
	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.33f,0.77f,0.0f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glTranslatef(-1.0f,-1.0f,0.0f);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.77f,0.2f,0.3f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadIdentity();
}

void P3::renderScale(Window *w)
{
	w->cs455_glLoadIdentity();

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.9f,0.5f,1.0f);
		w->cs455_glVertex3f(0.5f,0.4f,0.0f);
		w->cs455_glVertex3f(0.8f,0.4f,0.0f);
		w->cs455_glVertex3f(0.65f,0.9f,0.0f);
	w->cs455_glEnd();

	w->cs455_glScalef(0.8f,0.7f,1.0f);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(0.8f,0.7f,0.0f);
		w->cs455_glVertex3f(0.5f,0.1f,0.0f);
		w->cs455_glVertex3f(0.8f,0.1f,0.0f);
		w->cs455_glVertex3f(0.65f,0.4f,0.0f);
	w->cs455_glEnd();	

	w->cs455_glLoadIdentity();
}

void P3::renderOrtho(Window *w)
{
	w->cs455_glLoadIdentity();

	w->cs455_glOrtho(0,640,0,480,-1,1);

	w->cs455_glBegin(GL_TRIANGLES);
		w->cs455_glColor3f(1.0f,0.0f,0.0f);
		w->cs455_glVertex3f(300.0f,300.0f,0.0f);
		w->cs455_glColor3f(0.0f,1.0f,0.0f);
		w->cs455_glVertex3f(600.0f,300.0f,0.0f);
		w->cs455_glColor3f(0.0f,0.0f,1.0f);
		w->cs455_glVertex3f(450.0f,410.0f,0.0f);
		w->cs455_glColor3f(1.0f,1.0f,0.0f);
		w->cs455_glVertex3f(100.0f,400.0f,0.0f);
		w->cs455_glColor3f(0.0f,1.0f,1.0f);
		w->cs455_glVertex3f(70.0f,60.0f,0.0f);
		w->cs455_glColor3f(1.0f,0.0f,1.0f);
		w->cs455_glVertex3f(350.0f,100.0f,0.0f);
	w->cs455_glEnd();

	w->cs455_glLoadIdentity();
}

void P3::tree(Window *w, int depth)
{
	static const double r2 = 1/sqrt(2);
	static const double mdown[16] = { 0,-r2,0,0, r2,0,0,0, 0,0,1,0, 0,-r2,0,1 };
	static const double mup[16] = { 0,r2,0,0, -r2,0,0,0, 0,0,1,0, 0,r2,0,1 };

	if (depth <= 0) return;

	w->cs455_glBegin(GL_LINES);
		w->cs455_glVertex2f(0.0f, (float)-r2);
		w->cs455_glVertex2f(0.0f, (float)r2);
	w->cs455_glEnd();
	
	w->cs455_glPushMatrix();
	w->cs455_glMultMatrixd(mdown);
	
	tree(w, depth-1);
	
	w->cs455_glPopMatrix();
	w->cs455_glPushMatrix();
	w->cs455_glMultMatrixd(mup);
	
	tree(w, depth-1);
	
	w->cs455_glPopMatrix();
}
