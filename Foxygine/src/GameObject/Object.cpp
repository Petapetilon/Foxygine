#include "Object.h"


void Object::SetActive(bool active)
{
	isActive = active;
	if (active)
		OnEnable();
	else
		OnDisable();
}


bool Object::GetActiveSelf()
{
	return isActive;
}
