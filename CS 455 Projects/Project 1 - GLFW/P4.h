#pragma once

#include "Window.h"

class P4
{
public:
	static const int SCENE_COUNT = 2;

	static void redraw(Window *w);
	static void renderFrustum(Window *w);
	static void renderPerspective(Window *w);
};

