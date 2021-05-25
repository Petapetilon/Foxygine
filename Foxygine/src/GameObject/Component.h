#pragma once
#include <memory>
#include <typeinfo>
#include "Object.h"


class GameObject;
class Transform;

class Component : public Object
{
public:
	GameObject* gameObject;
	Transform* transform;
	bool isActive = true;

	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void FixedUpdate(float deltaTime) {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnPreRender() {}
	virtual void OnPostRender() {}
	virtual void OnTransformChanged() {}
};

