#pragma once
#include <memory>
#include <typeinfo>


class GameObject;
class Transform;

class Component
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
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnPreRender() {}
	virtual void OnPostRender() {}
	virtual void OnTransformChanged() {}
};

