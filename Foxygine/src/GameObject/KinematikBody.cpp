#include "KinematikBody.h"
#include "Transform.h"
#include "GameObject.h"


void KinematikBody::Update(float deltaTime)
{
	if (velocitySet) {
		if(terminate)
			if ((startPosition - transform->Position()).SqrMagnitude() > maxDistance) 
			gameObject->Destroy();
		
		transform->Translate(velocity * deltaTime);
	}

	if (angularSet) {
		transform->Rotate(axis, speed);
	}
}

void KinematikBody::SetKinematikVelocity(Vector3 _velocity, float length, bool terminateObject)
{
	velocity = _velocity;
	maxDistance = length;
	terminate = terminateObject;
	startPosition = transform->Position();
	velocitySet = true;
	gameObject->SetActive(true);
}

void KinematikBody::SetKinematikAngularVelocity(Vector3 _axis, float _speed)
{
	axis = _axis;
	speed = _speed;
	angularSet = true;
	gameObject->SetActive(true);
}
