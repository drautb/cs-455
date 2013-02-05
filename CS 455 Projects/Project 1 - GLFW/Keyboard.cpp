#include <GL/glfw.h>

#include "Keyboard.h"

void KeyboardCallback(int key, int keyState)
{
	Keyboard::GetInstance().keys[key] = keyState;
	Keyboard::GetInstance().keyLocks[key] = keyState;
}

Keyboard::Keyboard(void)
{
	for (int k=0; k<KEY_COUNT; k++)
		keyLocks[k] = false;
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

bool Keyboard::KeyIsHit(char key)
{
	return KeyIsHit((int)key);
}

bool Keyboard::KeyIsHit(int key)
{
	bool hit = keys[key] && keyLocks[key];

	keyLocks[key] = false;

	return hit;
}






