#pragma once

#include "Window.h"

class P4
{
public:
	static const int SCENE_COUNT = 7;

	static void redraw(Window *w);
	static void renderFrustum(Window *w);
	static void renderPerspective(Window *w);
	static void renderGouraud(Window *w);
	static void renderSpecular(Window *w);
	static void renderLinearFog(Window *w);
	static void renderExpFog(Window *w);
	static void renderExpSqFog(Window *w);
};

