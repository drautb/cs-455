#pragma once

#include "cs455Utils.h"

class Light
{
public:

	Vector455					position;		// This light's position
	Vector455					diffuse;		// This light's color properties
	Vector455					ambient;
	Vector455					specular;

public:
	/**
	 * Constructor/Destructor
	 */
	Light();

	~Light();

	void reset();
};
