#pragma once

#include <GL\glfw.h>

void KeyboardCallback(int, int);

class Keyboard
{
public:
	/**
	 * Class Constants
	 */
	const static int KEY_COUNT		= 256;

	/**
	 * Other Members
	 */
	int keys[KEY_COUNT];

private:
	/**
	 * Constructor
	 */
	Keyboard(void);

	// Not implemented
	Keyboard(Keyboard const &other);
	void operator=(Keyboard const &rhs);

public:
	/**
	 * Singleton Access
	 */
	static Keyboard &GetInstance()
	{
		static Keyboard instance;

		return instance;
	}

	/** 
	 * Destructor
	 */
	~Keyboard(void);

	/**
	 * Other Methods
	 */
	bool KeyIsDown(char key);

	bool KeyIsDown(int key);
};




