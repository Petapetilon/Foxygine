#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectHandler.h"


GameObject::GameObject(std::string _name)
{
	name = _name;
	transform = new Transform();
	isActive = true;
}


std::shared_ptr<GameObject> GameObject::CreateGameObject(std::string _name)
{
	return std::shared_ptr<GameObject>(GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject>(new GameObject(_name))));
}


std::shared_ptr<GameObject> GameObject::CreateGameObject()
{
	return std::shared_ptr<GameObject>(GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject>(new GameObject("unnamed"))));
}


std::shared_ptr<GameObject> GameObject::FindGameObject(std::string _name)
{
	return std::shared_ptr<GameObject>(GameObjectHandler::FindGameObject(_name));
}


bool GameObject::FindAllGameObjects(std::string _name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results)
{
	return GameObjectHandler::FindAllGameObjects(_name, results);
}


void GameObject::SetActive(bool active)
{
	isActive = active;
	if (active)
		OnEnable();
	else
		OnDisable();
}


bool GameObject::GetActiveSelf()
{
	return isActive;
}


void GameObject::OnEnable()
{
	GameObjectHandler::SetGameObjectActiveStatus(*this, true);
	for (auto compPtr : components) {
		compPtr->comp->OnEnable();
	}
}


void GameObject::OnDisable()
{
	GameObjectHandler::SetGameObjectActiveStatus(*this, false);
	for (auto compPtr : components) {
		compPtr->comp->OnDisable();
	}
}


void GameObject::Start()
{
	GameObjectHandler::SetGameObjectActiveStatus(*this, isActive);

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
