#include "Light.h"

Light::Light(void)
{
	reset();
}

Light::~Light(void)
{

}

void Light::reset(void) 
{
	position.Zero();
	position.z() = 1.0f;

	ambient.Zero();
	ambient.w() = 1.0f;

	diffuse.Zero();

	specular.Zero();
}
