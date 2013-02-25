#pragma once

#include "Window.h"

/**
 * Class to encapsulate rendering codes for Project 2
 */
class P2
{
public:
	static const int SCENE_COUNT = 8;

	static void redraw(Window *w);
	static void renderPoints(Window *w);
	static void renderLines(Window *w);
	static void renderPolygons(Window *w);
	static void renderLineStrip(Window *w);
	static void renderLineLoop(Window *w);
	static void renderTriangleStrip(Window *w);
	static void renderTriangleFan(Window *w);
	static void renderQuads(Window *w);
	static void renderQuadStrip(Window *w);
	static void renderFatLines(Window *w);
};

