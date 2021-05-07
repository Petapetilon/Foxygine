#include "SimpleCamera.h"
#include <iostream>



void SimpleCamera::Start()
{
	Mouse::RegisterOnMove(OnMouseMove);
	std::cout << "registereing to mouse movement";
}

void SimpleCamera::OnMouseMove(Vector2 pos)
{
	std::cout << pos.x << ", " << pos.y << std::endl;
}

void SimpleCamera::OnKeyPress(KeyCode, KeyState)
{
}
