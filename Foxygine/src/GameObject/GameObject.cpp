#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectHandler.h"


GameObject::GameObject(std::string _name)
{
	name = _name;
	transform = new Transform();
	GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject>(this));
}


void GameObject::SetActive(bool active)
{
	isActive = active;
	if (active)
		OnEnable();
	else
		OnDisable();
}


void GameObject::OnEnable()
{
	for (auto compPtr : components) {
		compPtr->comp->OnEnable();
	}
}


void GameObject::OnDisable()
{
	for (auto compPtr : components) {
		compPtr->comp->OnDisable();
	}
}


void GameObject::Start()
{
	for (auto compPtr : components) {
		if(compPtr->comp->isActive)
			compPtr->comp->Start();
	}
}


void GameObject::Update(float deltaTime)
{
	for (auto compPtr : components) {
		if (compPtr->comp->isActive)
			compPtr->comp->Update(deltaTime);
	}
}


void GameObject::FixedUpdate(float deltaTime)
{
	for (auto compPtr : components) {
		if (compPtr->comp->isActive)
			compPtr->comp->FixedUpdate(deltaTime);
	}
}


void GameObject::OnPreRender()
{
	for (auto compPtr : components) {
		compPtr->comp->OnPreRender();
	}
}


void GameObject::OnPostRender()
{
	for (auto compPtr : components) {
		compPtr->comp->OnPostRender();
	}
}


void GameObject::OnTransformChanged()
{
	for (auto compPtr : components) {
		compPtr->comp->OnTransformChanged();
	}
}
