#pragma once
#include "Component.h"
#include "../Peripherals/Mouse.h"
#include "../Peripherals/Keyboard.h"


class SimpleCamera : public Component
{
public:
	void Start() override;

	static void OnMouseMove(Vector2);
	void OnKeyPress(KeyCode, KeyState);
};

