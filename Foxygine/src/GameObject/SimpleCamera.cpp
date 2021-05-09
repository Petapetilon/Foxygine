#include "SimpleCamera.h"
#include <iostream>
#include "../Math/Vector3.h"
#include "Transform.h"



void SimpleCamera::Start()
{

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


	if (Keyboard::GetKey(KeyCode::W)) {
		transform->Translate(transform->Forward() * deltaTime * 10);
	}	
	
	if (Keyboard::GetKey(KeyCode::S)) {
		transform->Translate(transform->Forward() * -deltaTime * 10);
	}
	
	if (Keyboard::GetKey(KeyCode::D)) {
		transform->Translate(transform->Right() * deltaTime * 10);
	}

	if (Keyboard::GetKey(KeyCode::A)) {
		transform->Translate(transform->Right() * -deltaTime * 10);
	}	

	if (Keyboard::GetKey(KeyCode::Space)) {
		transform->Translate(transform->Up() * deltaTime * 10);
	}	
	
	if (Keyboard::GetKey(KeyCode::L_Shift)) {
		transform->Translate(transform->Up() * -deltaTime * 10);
	}
}
