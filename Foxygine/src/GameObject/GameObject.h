#pragma once
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include "Component.h"
#include "Object.h"


class Transform;

class GameObject : public Object
{
private:
	class ComponentNode {
	public:
		ComponentNode(Component* component, std::string typeID) {
			comp = component;
			compTypeName = typeID;
		}

		ComponentNode() {}

		Component* comp;
		std::string compTypeName;
	};


	std::list<std::shared_ptr<ComponentNode>> components;

	GameObject(std::string _name);

public:
	Transform* transform;
	long uniqueID;
	
	
	static std::shared_ptr<GameObject> CreateGameObject(std::string _name);
	static std::shared_ptr<GameObject> CreateGameObject();
	static std::shared_ptr<GameObject> FindGameObject(std::string _name);
	static bool FindAllGameObjects(std::string _name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results);


	template<class Comp>
	bool AddComponent(Comp* component)
	{
		static_assert(std::is_base_of<Component, Comp>::value);

		for (auto compPtr : components) {
			if (compPtr->compTypeName == typeid(Comp*).name()) {
				std::cout << "Unable to bind <" << typeid(Comp).name() << "> to <" << name << "> !" << std::endl;
				std::cout << "Component Already Bound";
				return false;
			}
		}
		
		components.push_back(std::make_shared<ComponentNode>(ComponentNode(component, typeid(Comp*).name())));
		((Component*)component)->gameObject = this;
		((Component*)component)->transform = transform;
		((Component*)component)->OnAttach();
		return true;
	}


	template<class Comp>
	Comp* GetComponent() {
		for (auto compPtr : components) {
			if (compPtr->compTypeName == typeid(Comp*).name()) {
				return (Comp*)compPtr->comp;
			}
		}

		return nullptr;
	}


	template<class Comp>
	bool TryGetComponent(Comp* foundComp) {
		for (auto compPtr : components) {
			if (compPtr->compTypeName == typeid(Comp*).name()) {
				foundComp = compPtr->comp;
				return true;
			}
		}

		foundComp = nullptr;
		return false;
	}


	template<class Comp>
	Comp* RemoveComponent()
	{
		for (auto compPtr : components) {
			if (compPtr->compTypeName == typeid(Comp*).name()) {
				components.remove(compPtr);
				compPtr->comp->gameObject = nullptr;
				compPtr->comp->transform = nullptr;
				compPtr->comp->OnDetach();
				return compPtr->comp;
			}
		}
	}


	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void Start();
	void Update(float);
	void FixedUpdate(float);
	void OnPreRender();
	void OnPostRender();
	void OnTransformChanged();
};
