#pragma once
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include "Component.h"
#include "Object.h"
#include "../Math/Vector3.h"


class Transform;

class GameObject : public Object
{
private:
	class ComponentNode {
	public:		
		ComponentNode(Component* component, std::size_t typeHash) {
			comp = component;
			compTypeHash = typeHash;
		}

		ComponentNode() {}
		~ComponentNode() {
			delete comp;
		}

		Component* comp;
		std::size_t compTypeHash;
	};


	std::list<std::shared_ptr<ComponentNode>> components;
	bool markedForDestroy;

	GameObject(std::string _name);

public:
	Transform* transform;
	long uniqueID;


	~GameObject();
	
	
	static std::shared_ptr<GameObject> CreateGameObject(std::string _name);
	static std::shared_ptr<GameObject> CreateGameObject();
	static std::shared_ptr<GameObject> CreateInstance(std::shared_ptr<GameObject> original);
	static std::shared_ptr<GameObject> CreateLinkedInstance(std::shared_ptr<GameObject> original);
	static std::shared_ptr<GameObject> FindGameObject(std::string _name);
	static bool FindAllGameObjects(std::string _name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results);


	template<class Comp>
	bool AddComponent(Comp* component)
	{
		static_assert(std::is_base_of<Component, Comp>::value);

		for (auto compPtr : components) {
			if (compPtr->compTypeHash == typeid(Comp*).hash_code()) {
				std::cout << "Unable to bind <" << typeid(Comp).name() << "> to <" << name << "> !" << std::endl;
				std::cout << "Component Already Bound";
				return false;
			}
		}
		
		components.push_back(std::shared_ptr<ComponentNode>(new ComponentNode(component, typeid(Comp*).hash_code())));
		((Component*)component)->gameObject = this;
		((Component*)component)->transform = transform;
		((Component*)component)->OnAttach();
		return true;
	}


	template<class Comp>
	Comp* GetComponent() {
		for (auto compPtr : components) {
			if (compPtr->compTypeHash == typeid(Comp*).hash_code()) {
				return (Comp*)compPtr->comp;
			}
		}

		return nullptr;
	}


	template<class Comp>
	bool TryGetComponent(Comp* foundComp) {
		for (auto compPtr : components) {
			if (compPtr->compTypeHash == typeid(Comp*).hash_code()) {
				foundComp = (Comp*)compPtr->comp;
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
			if (compPtr->compTypeHash == typeid(Comp*).hash_code()) {
				components.remove(compPtr);
				compPtr->comp->gameObject = nullptr;
				compPtr->comp->transform = nullptr;
				compPtr->comp->OnDetach();
				return (Comp*)compPtr->comp;
			}
		}
	}


	virtual void SetActive(bool active) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void AddComponent(Component* comp, std::size_t compHash);

	void Start();
	void Update(float);
	void FixedUpdate(float);
	void OnPreRender();
	void OnPostRender();
	void OnTransformChanged();
	void Destroy();
};
