#pragma once
#include <vector>
#include <stack>
#include <queue>
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
	static std::list<GameObject*> gameObjectList;
	static std::queue<GameObject*> toRegObjects;
	static std::queue<GameObject*> toToggleObjects;
	static std::queue<bool> toToggleObjectsStatus;
	static std::map<long, std::shared_ptr<GameObject>> activeGameObjects;
	static std::list<GameObject*> activeGameObjectList;
	static std::vector<long> toDeleteObjects;
	static std::vector<std::string> toDeleteNames;
	static unsigned long startUpTime;
	static unsigned long currentSystemTime;
	static unsigned long lastUpdateSystemTime;
	static unsigned long lastFixedUpdateSystemTime;
	static long latestUniqueGameObjectID;

	static long HashString(std::string);
	static void RegisterObjects();
	static void DeleteObjects();
	static void ToggleObjects();

public:
	static double lastUpdateDeltaTime;
	static double lastFixedUpdateDeltaTime;
	static double timeSinceStartup;

	static std::shared_ptr<GameObject> RegisterGameObject(std::shared_ptr<GameObject>);
	static void UnregisterGameObject(GameObject*);
	static void SetGameObjectActiveStatus(GameObject&, bool);
	static void TickHandler(Foxygine&);
	static void InitHandler();
	static void InvokeOnPreRender();
	static void InvokeOnPostRender();

	static std::shared_ptr<GameObject> FindGameObject(std::string name);
	static bool FindAllGameObjects(std::string name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results);

private:
	static void InvokeUpdate(float, Foxygine const &);
	static void InvokeFixedUpdate(float, Foxygine const &);
	static void UpdateThread(Foxygine const &);
};

