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
	position = Vector455::Zero();
	position.z() = 1.0f;

	ambient = Vector455::Zero();
	ambient.w() = 1.0f;

	diffuse = Vector455::Zero();

	specular = Vector455::Zero();
}
