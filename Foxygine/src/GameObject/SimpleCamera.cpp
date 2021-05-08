#include "SimpleCamera.h"
#include <iostream>
#include "../Math/Vector3.h"
#include "Transform.h"



void SimpleCamera::Start()
{
	//Mouse::RegisterOnMove(std::bind(&SimpleCamera::OnMouseMove, this, std::placeholders::_1));
	//Keyboard::RegisterOnAnyKeyCallback(std::bind(&SimpleCamera::OnKeyPress, this, std::placeholders::_2));
}

void SimpleCamera::Update(float _deltaTime)
{
	deltaTime = _deltaTime;
	xRot += Mouse::Velocity().x * deltaTime * 10;
	yRot += Mouse::Velocity().y * deltaTime * 10;
	if (yRot > 89) yRot = 89;
	if (yRot < -89) yRot = -89;


	transform->SetRotation(yRot, 0, 0, false);
	transform->RotateAround(Vector3(0,0,0), Vector3(0, 1, 0), xRot);


	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_W)) {
		transform->Translate(transform->Forward() * deltaTime * 10);
	}	
	
	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_S)) {
		transform->Translate(transform->Forward() * -deltaTime * 10);
	}
	
	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_D)) {
		transform->Translate(transform->Right() * deltaTime * 10);
	}

	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_A)) {
		transform->Translate(transform->Right() * -deltaTime * 10);
	}	

	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_SPACE)) {
		transform->Translate(transform->Up() * deltaTime * 10);
	}	
	
	if (glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), GLFW_KEY_LEFT_SHIFT)) {
		transform->Translate(transform->Up() * -deltaTime * 10);
	}
}
