#include "P2.h"
#include "Window.h"

/**
 * Project rendering methods
 */
void P2::redraw(Window *w)
{
	int sceneToRender = w->SceneToRender();
	if (sceneToRender < 0) sceneToRender *= -1;
	sceneToRender %= SCENE_COUNT;

	if (sceneToRender == 0)
	{
		renderPoints(w);
		renderLines(w);
		renderPolygons(w);
	}
	else if (sceneToRender == 1)
		renderLineStrip(w);
	else if (sceneToRender == 2)
		renderLineLoop(w);
	else if (sceneToRender == 3)
		renderTriangleStrip(w);
	else if (sceneToRender == 4)
		renderTriangleFan(w);
	else if (sceneToRender == 5)
		renderQuads(w);
	else if (sceneToRender == 6)
		renderQuadStrip(w);
	else if (sceneToRender == 7)
		renderFatLines(w);
}

void P2::renderPoints(Window *w)
{	
	w->cs455_glBegin(GL_POINTS);

	for(float theta=0, radius=60.0f; radius>1.0f; theta+=0.1f, radius-=0.3f)
	{
		w->cs455_glColor3f(radius/60.0f,0.3f,1.0f-(radius/60.0f));
		w->cs455_glVertex2i((int)(80.0f+radius*cos(theta)),(int)(400.0f+radius*sin(theta)));
	}
	
	w->cs455_glEnd();
}

void P2::renderLines(Window *w)
{
	w->cs455_glBegin(GL_LINES);

	w->cs455_glColor3f(1,1,1);
	w->cs455_glVertex2i(0,0);
	w->cs455_glVertex2i(100, 50);

	for(int i=0; i<=8; i++)
	{
		w->cs455_glColor3f(1,0,0);
		w->cs455_glVertex2i(100,250);
		w->cs455_glVertex2i(100 + 10*i, 330);
		w->cs455_glColor3f(0,1,0);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(0,1,1);
		w->cs455_glVertex2i(100 - 10*i, 330);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(0,0,1);
		w->cs455_glVertex2i(180, 250 + 10*i);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(1,0,1);
		w->cs455_glVertex2i(180, 250 - 10*i);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(1,1,1);
		w->cs455_glVertex2i(20, 250 + 10 * i);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(1,1,0);
		w->cs455_glVertex2i(20, 250 - 10 * i);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(0,1,0);
		w->cs455_glVertex2i(100 + 10 * i, 170);
		w->cs455_glVertex2i(100,250);
		w->cs455_glColor3f(1,0,0);
		w->cs455_glVertex2i(100 - 10 * i, 170);
	}

	w->cs455_glEnd();
}

void P2::renderPolygons(Window *w)
{
	w->cs455_glBegin(GL_TRIANGLES);
 
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(500,400);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(600,400);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(550,460);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(300,460);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(270,310);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(550,350);

	w->cs455_glEnd();
}

void P2::renderLineStrip(Window *w)
{
	w->cs455_glBegin(GL_LINE_STRIP);

	w->cs455_glColor3f(0.42f,0.27f,0.11f);
	w->cs455_glVertex2i(250,30);
	w->cs455_glVertex2i(270,60);
	w->cs455_glVertex2i(270,210);
	w->cs455_glColor3f(0.04f,0.70f,0.02f);
	w->cs455_glVertex2i(230,230);
	w->cs455_glVertex2i(220,270);
	w->cs455_glVertex2i(220,310);
	w->cs455_glVertex2i(250,340);
	w->cs455_glVertex2i(275,360);
	w->cs455_glVertex2i(325,360);
	w->cs455_glVertex2i(350,340);
	w->cs455_glVertex2i(380,310);
	w->cs455_glVertex2i(380,270);
	w->cs455_glVertex2i(370,230);
	w->cs455_glColor3f(0.42f,0.27f,0.11f);
	w->cs455_glVertex2i(330,210);
	w->cs455_glVertex2i(330,60);
	w->cs455_glVertex2i(350,30);

	w->cs455_glEnd();
}

void P2::renderLineLoop(Window *w)
{
	w->cs455_glBegin(GL_LINE_LOOP);

	w->cs455_glColor3f(0.42f,0.27f,0.11f);
	w->cs455_glVertex2i(250,30);
	w->cs455_glVertex2i(270,60);
	w->cs455_glVertex2i(270,210);
	w->cs455_glColor3f(0.04f,0.70f,0.02f);
	w->cs455_glVertex2i(230,230);
	w->cs455_glVertex2i(220,270);
	w->cs455_glVertex2i(220,310);
	w->cs455_glVertex2i(250,340);
	w->cs455_glVertex2i(275,360);
	w->cs455_glVertex2i(325,360);
	w->cs455_glVertex2i(350,340);
	w->cs455_glVertex2i(380,310);
	w->cs455_glVertex2i(380,270);
	w->cs455_glVertex2i(370,230);
	w->cs455_glColor3f(0.42f,0.27f,0.11f);
	w->cs455_glVertex2i(330,210);
	w->cs455_glVertex2i(330,60);
	w->cs455_glVertex2i(350,30);

	w->cs455_glEnd();
}

void P2::renderTriangleStrip(Window *w)
{
	w->cs455_glBegin(GL_TRIANGLE_STRIP);

	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(40,70);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(40,390);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(130,30);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(130,350);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(330,80);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(330,400);
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(480,40);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(530,330);

	w->cs455_glEnd();
}

void P2::renderTriangleFan(Window *w)
{
	w->cs455_glBegin(GL_TRIANGLE_FAN);

	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(250,170);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(400,140);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(300,50);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(175,55);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(100,170);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(175,285);
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(300,290);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(400,200);

	w->cs455_glEnd();
}

void P2::renderQuads(Window *w)
{
	w->cs455_glBegin(GL_QUADS);

	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(40,70);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(40,390);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(130,350);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(130,30);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(330,80);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(330,400);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(530,330);
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(480,40);

	w->cs455_glEnd();
}

void P2::renderQuadStrip(Window *w)
{
	w->cs455_glBegin(GL_QUAD_STRIP);

	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(40,70);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(40,390);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(130,30);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(130,350);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(330,80);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(330,400);
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(480,40);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(530,330);

	w->cs455_glEnd();
}

void P2::renderFatLines(Window *w)
{
	w->cs455_glLineWidth(5);
	
	w->cs455_glBegin(GL_LINES);
 
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(200,270);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(225,300);
	w->cs455_glVertex2i(225,300);
	w->cs455_glColor3f(1,1,0);
	w->cs455_glVertex2i(255,300);
	w->cs455_glVertex2i(255,300);
	w->cs455_glColor3f(0,0,1);
	w->cs455_glVertex2i(280,270);
	w->cs455_glVertex2i(280,270);
	w->cs455_glColor3f(1,0,1);
	w->cs455_glVertex2i(280,230);
	w->cs455_glVertex2i(280,230);
	w->cs455_glColor3f(0,1,1);
	w->cs455_glVertex2i(240,190);
	w->cs455_glVertex2i(240,190);
	w->cs455_glColor3f(1,0,0);
	w->cs455_glVertex2i(240,160);
	w->cs455_glVertex2i(240,150);
	w->cs455_glColor3f(0,1,0);
	w->cs455_glVertex2i(240,145);

	w->cs455_glEnd();

	w->cs455_glLineWidth(1);
}
