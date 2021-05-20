#pragma once
#include <list>
#include <map>
#include <memory>
#include <chrono>
#include <thread>
#include "GameObject.h"


class Foxygine;


class GameObjectHandler
{
private:
	static std::map<long, long> gameObjectIDLookup;
	static std::map<long, std::shared_ptr<GameObject>> gameObjects;
	static std::map<long, std::shared_ptr<GameObject>> activeGameObjects;
	static unsigned long currentSystemTime;
	static unsigned long lastUpdateSystemTime;
	static unsigned long lastFixedUpdateSystemTime;
	static long latestUniqueGameObjectID;

	static long HashString(std::string);

public:
	static double lastUpdateDeltaTime;
	static double lastFixedUpdateDeltaTime;

	static std::shared_ptr<GameObject> RegisterGameObject(std::shared_ptr<GameObject>);
	static void UnregisterGameObject(std::shared_ptr<GameObject>);
	static void SetGameObjectActiveStatus(GameObject&, bool);
	static void TickHandler(Foxygine&);
	static void InitHandler();
	static void InvokeOnPreRender();
	static void InvokeOnPostRender();

	static std::shared_ptr<GameObject> FindGameObject(std::string name);
	static bool FindAllGameObjects(std::string name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results);

private:
	static void InvokeUpdate(float, Foxygine&);
	static void InvokeFixedUpdate(float, Foxygine&);
};

