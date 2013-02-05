#pragma once

#include "cs455Utils.h"

class PointColor
{
public:
	int				x;
	int				y;
	Vector455		color;

public:

	/**
	 * Constructor/Destructor
	 */
	PointColor(void)
	{
		/*
		x = new int();
		y = new int();
		color = new Vector455();

		x = -1;
		y = -1;
		color->Zero();
		*/

		x = -1;
		y = -1;
		color.Zero();
	}

	PointColor(int x, int y, Vector455& color)
	{
		this->x = x;
		this->y = y;
		this->color = color;
	}

	~PointColor(void)
	{
		/*
		SAFE_DELETE(x);
		SAFE_DELETE(y);
		SAFE_DELETE(color);
		*/
	}

};

