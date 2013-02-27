#pragma once

#include "Window.h"

/**
 * Class to encapsulate project 3 rendering code
 */
class P3
{
public:
	static const int SCENE_COUNT = 12;

	static void redraw(Window *w);
	static void renderViewport(Window *w);
	static void render1OverW(Window *w);
	static void renderDepthBuffer(Window *w);
	static void renderEasyPointClipping(Window *w);
	static void renderMatrixManips(Window *w);
	static void renderMatrixStacks(Window *w);
	static void renderRotate(Window *w);
	static void renderTranslate(Window *w);
	static void renderScale(Window *w);
	static void renderOrtho(Window *w);
	static void renderLookAt(Window *w);
	static void renderScaleFix(Window *w);
	static void tree(Window *w, int depth);
};

