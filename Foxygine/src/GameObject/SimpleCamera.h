#pragma once
#include "Component.h"
#include "../Peripherals/Mouse.h"
#include "../Peripherals/Keyboard.h"


class SimpleCamera : public Component
{
private:
	float deltaTime;
	float xRot;
	float yRot;

public:
	void Start() override;
	void Update(float _deltaTime) override;
};

