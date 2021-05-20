#include "GameObjectHandler.h"
#include "../Graphics/Graphics.h"
#include "../Foxygine.h"
#include <iomanip>


std::map<long, long> GameObjectHandler::gameObjectIDLookup;
std::map<long, std::shared_ptr<GameObject>> GameObjectHandler::gameObjects;
std::map<long, std::shared_ptr<GameObject>> GameObjectHandler::activeGameObjects;
unsigned long GameObjectHandler::currentSystemTime;
unsigned long GameObjectHandler::lastUpdateSystemTime;
unsigned long GameObjectHandler::lastFixedUpdateSystemTime;
long GameObjectHandler::latestUniqueGameObjectID;
double GameObjectHandler::lastUpdateDeltaTime;
double GameObjectHandler::lastFixedUpdateDeltaTime;


long GameObjectHandler::HashString(std::string string)
{
	long value = 0;
	for (int i = 0; i < string.length() - 1; i += 2) {
		value <<= 8;
		value |= string.c_str()[i] ^ string.c_str()[i + 1];
	}

	return value;
}


std::shared_ptr<GameObject> GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects[++latestUniqueGameObjectID] = gameObject;
	gameObject->uniqueID = latestUniqueGameObjectID;

#ifdef USE_NAME_LOOKUP
	gameObjectIDLookup[HashString(gameObject->name)] = latestUniqueGameObjectID;
#endif

	return std::shared_ptr<GameObject>(gameObject);
}


void GameObjectHandler::UnregisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.erase(gameObject->uniqueID);
	gameObject->uniqueID = -1;
}

void GameObjectHandler::SetGameObjectActiveStatus(GameObject& gameObject, bool status)
{
	if (status)
		activeGameObjects[gameObject.uniqueID] = gameObjects[gameObject.uniqueID];
	else
		activeGameObjects.erase(gameObject.uniqueID);
}


void GameObjectHandler::TickHandler(Foxygine& foxygine)
{
	//Measures Time
	currentSystemTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	//Frame Update
	InvokeOnPreRender();
	Graphics::RenderFrame();
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
		go.second->Start();
	}

	lastUpdateSystemTime = currentSystemTime;
	lastFixedUpdateSystemTime = currentSystemTime;
}


void GameObjectHandler::InvokeUpdate(float deltaTime, Foxygine& foxy)
{
	foxy.UpdateFoxygine(deltaTime);

	for (auto go : activeGameObjects) {
		go.second->Update(deltaTime);
	}
}


void GameObjectHandler::InvokeFixedUpdate(float deltaTime, Foxygine& foxy)
{
	Mouse::Tick(deltaTime);
	foxy.FixedUpdateFoxygine(deltaTime);

	for (auto go : gameObjects) {
		go.second->FixedUpdate(deltaTime);
	}
}


void GameObjectHandler::InvokeOnPreRender()
{
	for (auto go : activeGameObjects) {
		go.second->OnPreRender();
	}
}


void GameObjectHandler::InvokeOnPostRender()
{
	for (auto go : activeGameObjects) {
		go.second->OnPostRender();
	}
}


std::shared_ptr<GameObject> GameObjectHandler::FindGameObject(std::string name)
{
#ifdef USE_NAME_LOOKUP
	return gameObjects[gameObjectIDLookup[HashString(name)]];
#else
	for (auto go : gameObjects) {
		if (name == go.second->name) {
			return go.second;
		}
	}
#endif
}

bool GameObjectHandler::FindAllGameObjects(std::string name, std::shared_ptr<std::list<std::shared_ptr<GameObject>>> results)
{
	bool found = false;
	for (auto go : gameObjects) {
		if (name == go.second->name) {
			results->push_back(std::shared_ptr<GameObject>(go.second));
			found = true;
		}
	}

	return found;
}
