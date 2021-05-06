#include "GameObjectHandler.h"
#include "../Graphics/Graphics.h"
#include "../Foxygine.h"
#include <iomanip>


std::list<std::shared_ptr<GameObject>> GameObjectHandler::gameObjects;
unsigned long GameObjectHandler::currentSystemTime;
unsigned long GameObjectHandler::lastUpdateSystemTime;
unsigned long GameObjectHandler::lastFixedUpdateSystemTime;
double GameObjectHandler::lastUpdateDeltaTime;
double GameObjectHandler::lastFixedUpdateDeltaTime;


void GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	for (auto go : gameObjects) {
		if (go->name == gameObject->name) {
			return;
		}
	}

	gameObjects.push_back(gameObject);
}


void GameObjectHandler::UnregisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	for (auto go : gameObjects) {
		if (go->name == gameObject->name) {
			gameObjects.remove(go);
		}
	}
}


void GameObjectHandler::TickHandler(Foxygine& foxygine)
{
	//Measures Time
	currentSystemTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	//Frame Update
	InvokeOnPreRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Graphics::DrawRenderer();
	InvokeOnPostRender();
	Keyboard::JoinInputThreads();
	Mouse::JoinInputThreads();
	lastUpdateDeltaTime = (double)(currentSystemTime - lastUpdateSystemTime) / 1000.0;
	InvokeUpdate((float)lastUpdateDeltaTime, foxygine);

	//Fixed Update
	if (currentSystemTime - lastFixedUpdateSystemTime >= 19) {
		lastFixedUpdateDeltaTime = (currentSystemTime - lastFixedUpdateSystemTime) / 1000.0;
		lastFixedUpdateSystemTime = currentSystemTime;
		InvokeFixedUpdate((float)lastFixedUpdateDeltaTime, foxygine);
	}

	lastUpdateSystemTime = currentSystemTime;
}


void GameObjectHandler::InitHandler()
{
	currentSystemTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	for (auto go : gameObjects) {
		go->Start();
	}

	lastUpdateSystemTime = currentSystemTime;
	lastFixedUpdateSystemTime = currentSystemTime;
}


void GameObjectHandler::InvokeUpdate(float deltaTime, Foxygine& foxy)
{
	foxy.UpdateFoxygine(deltaTime);

	for (auto go : gameObjects) {
		go->Update(deltaTime);
	}

	//std::cout << "\x1B[2J\x1B[H";
	//std::cout << "FrameTime: " << deltaTime;
}


void GameObjectHandler::InvokeFixedUpdate(float deltaTime, Foxygine& foxy)
{
	foxy.FixedUpdateFoxygine(deltaTime);

	for (auto go : gameObjects) {
		go->FixedUpdate(deltaTime);
	}

	//std::cout << "\x1B[2J\x1B[H";
	//std::cout << "FixedTime: " << deltaTime;
}


void GameObjectHandler::InvokeOnPreRender()
{
	for (auto go : gameObjects) {
		go->OnPreRender();
	}
}


void GameObjectHandler::InvokeOnPostRender()
{
	for (auto go : gameObjects) {
		go->OnPostRender();
	}
}


std::shared_ptr<GameObject> GameObjectHandler::GetGameObject(std::string _name)
{
	for (auto go : gameObjects) {
		if (_name == go->name) {
			return go;
		}
	}
}
