#include <GL/glfw.h>

#include "Keyboard.h"

void KeyboardCallback(int key, int keyState)
{
	Keyboard::GetInstance().keys[key] = keyState;
}

Keyboard::Keyboard(void)
{
	
}

/** 
 * Destructor
 */
Keyboard::~Keyboard(void)
{

}

/**
 * Other Methods
 */
bool Keyboard::KeyIsDown(char key)
{
	return KeyIsDown((int)key);
}

bool Keyboard::KeyIsDown(int key)
{
	return keys[key] == GLFW_PRESS;
}