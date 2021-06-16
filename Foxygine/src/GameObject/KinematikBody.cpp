#include "KinematikBody.h"
#include "Transform.h"
#include "GameObject.h"


void KinematikBody::FixedUpdate(float deltaTime)
{
	if (velocitySet) {
		if (terminate)
			if ((startPosition - transform->Position()).SqrMagnitude() > maxDistance * maxDistance) {
				gameObject->Destroy();
				return;
			}

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


Component* KinematikBody::Copy(std::size_t& compHash)
{
	compHash = typeid(this).hash_code();
	return new KinematikBody();
}


Component* KinematikBody::CopyLinked(std::size_t& compHash)
{
	compHash = typeid(this).hash_code();
	return new KinematikBody();
}
