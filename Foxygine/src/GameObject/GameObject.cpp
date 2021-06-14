#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectHandler.h"


GameObject::GameObject(std::string _name)
{
	name = _name;
	transform = new Transform();
	transform->gameObject = this;
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

std::shared_ptr<GameObject> GameObject::CreateInstance(std::shared_ptr<GameObject> original)
{
	auto go = CreateGameObject(original->name + "-Copy");

	original->transform->CopyTo(go->transform);

	std::size_t currentCopiedHash;
	Component* currentCopiedComp;
	for (auto comp : original->components) {
		currentCopiedComp = comp->comp->Copy(currentCopiedHash);
		
		if (currentCopiedHash != -1) 
			go->AddComponent(currentCopiedComp, currentCopiedHash);
	}

	return go;
}


std::shared_ptr<GameObject> GameObject::CreateInstance(std::shared_ptr<GameObject> original, Vector3 position)
{
	auto go = CreateInstance(original);
	go->transform->SetPosition(position);
	return go;
}

std::shared_ptr<GameObject> GameObject::CreateInstance(std::shared_ptr<GameObject> original, Vector3 position, Vector3 rotation)
{
	auto go = CreateInstance(original);
	go->transform->SetPosition(position);
	go->transform->SetRotation(rotation);
	return go;
}

std::shared_ptr<GameObject> GameObject::FindGameObject(std::string _name)
{
	return std::shared_ptr<GameObject>(GameObjectHandler::FindGameObject(_name));
}


bool GameObject::FindAllGameObjects(std::string _name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results)
{
	return GameObjectHandler::FindAllGameObjects(_name, results);
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


void GameObject::AddComponent(Component* comp, std::size_t compHash)
{
	components.push_back(std::make_shared<ComponentNode>(ComponentNode(comp, compHash)));
	comp->gameObject = this;
	comp->transform = transform;
	comp->OnAttach();
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


void GameObject::Destroy()
{
	//for (auto comp : components)
	//	comp->Destroy();
	//
	//components.~list();
	//delete[] transform;
	//GameObjectHandler::UnregisterGameObject(std::shared_ptr<GameObject>(this));
}
