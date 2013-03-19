#pragma once

#include "cs455Utils.h"

class PointColor
{
public:
	int				x;
	int				y;
	float			z;
	Vector455		color;
	Vector455		normal;

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

		Reset();
	}

	PointColor(int x, int y, Vector455& color)
	{
		this->x = x;
		this->y = y;
		this->z = 0.0f;
		this->color = color;
	}

	PointColor(int x, int y, float z, Vector455& color)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->color = color;
	}

	PointColor(int x, int y, float z, Vector455&color, Vector455& normal)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->color = color;
		this->normal = normal;
	}

	~PointColor(void)
	{
		/*
		SAFE_DELETE(x);
		SAFE_DELETE(y);
		SAFE_DELETE(color);
		*/
	}

	void Reset()
	{
		x = -1;
		y = -1;
		z = 0.0f;
		color.Zero();

		normal.Zero();
		normal.z() = 1.0f;
	}
};

