#pragma once
#include <list>
#include <memory>
#include <chrono>
#include <thread>
#include "GameObject.h"


class Foxygine;

class GameObjectHandler
{
private:
	static std::list<std::shared_ptr<GameObject>> gameObjects;
	static unsigned long currentSystemTime;
	static unsigned long lastUpdateSystemTime;
	static unsigned long lastFixedUpdateSystemTime;

public:
	static double lastUpdateDeltaTime;
	static double lastFixedUpdateDeltaTime;

	static void RegisterGameObject(std::shared_ptr<GameObject>);
	static void UnregisterGameObject(std::shared_ptr<GameObject>);
	static void TickHandler(Foxygine&);
	static void InitHandler();
	static void InvokeOnPreRender();
	static void InvokeOnPostRender();

	static std::shared_ptr<GameObject> GetGameObject(std::string);

private:
	static void InvokeUpdate(float, Foxygine&);
	static void InvokeFixedUpdate(float, Foxygine&);
};

