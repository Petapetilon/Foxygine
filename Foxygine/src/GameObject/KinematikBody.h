#pragma once
#include "Component.h"
#include "../Math/Vector3.h"


class KinematikBody : public Component
{
private:
	Vector3 startPosition;
	Vector3 velocity;
	float maxDistance;

	Vector3 axis;
	float speed;

	bool velocitySet;
	bool angularSet;
	bool terminate;

public:
	void FixedUpdate(float deltaTime) override;
	void SetKinematikVelocity(Vector3 velocity, float length, bool terminateObject = false);
	void SetKinematikAngularVelocity(Vector3 axis, float speed);

	Component* Copy(std::size_t& compHash) override;
	Component* CopyLinked(std::size_t& compHash) override;
};

