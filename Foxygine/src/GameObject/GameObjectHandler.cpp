#include "GameObjectHandler.h"
#include "../Graphics/Graphics.h"
#include "../Foxygine Include.h"
#include <iomanip>
#include <execution>
#include <algorithm>


std::map<long, long> GameObjectHandler::gameObjectIDLookup;
std::map<long, std::shared_ptr<GameObject>> GameObjectHandler::gameObjects;
std::list<GameObject*> GameObjectHandler::gameObjectList;
std::queue<GameObject*> GameObjectHandler::toRegObjects;
std::queue<GameObject*> GameObjectHandler::toToggleObjects;
std::queue<bool> GameObjectHandler::toToggleObjectsStatus;
std::map<long, std::shared_ptr<GameObject>> GameObjectHandler::activeGameObjects;
std::list<GameObject*> GameObjectHandler::activeGameObjectList;
std::vector<long> GameObjectHandler::toDeleteObjects;
std::vector<std::string> GameObjectHandler::toDeleteNames;
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


void GameObjectHandler::DeleteObjects()
{
	while (!toDeleteObjects.empty()) {
		gameObjectList.remove(gameObjects[toDeleteObjects.back()].get());
		//activeGameObjectList.remove(activeGameObjects[toDeleteObjects.back()]);
		gameObjects.erase(toDeleteObjects.back());
		activeGameObjects.erase(toDeleteObjects.back());
		toDeleteObjects.pop_back();

		//gameObjectIDLookup.erase(HashString(toDeleteNames.back()));
		toDeleteNames.pop_back();
	}
}


void GameObjectHandler::RegisterObjects()
{
	while (!toRegObjects.empty()) {
		gameObjectList.push_back(toRegObjects.front());
		toRegObjects.pop();
	}
}


void GameObjectHandler::ToggleObjects()
{
	while(!toToggleObjects.empty()){
		if (toToggleObjectsStatus.front()) {
			activeGameObjects[toToggleObjects.front()->uniqueID] = gameObjects[toToggleObjects.front()->uniqueID];
			activeGameObjectList.push_back(toToggleObjects.front());
		}
		else {
			activeGameObjects.erase(toToggleObjects.front()->uniqueID);
			activeGameObjectList.remove(toToggleObjects.front());
		}

		toToggleObjects.pop();
		toToggleObjectsStatus.pop();
	}
}


std::shared_ptr<GameObject> GameObjectHandler::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	toRegObjects.push(gameObject.get());
	gameObjects[++latestUniqueGameObjectID] = gameObject;
	gameObject->uniqueID = latestUniqueGameObjectID;

#ifdef USE_NAME_LOOKUP
	gameObjectIDLookup[HashString(gameObject->name)] = latestUniqueGameObjectID;
#endif

	return std::shared_ptr<GameObject>(gameObject);
}


void GameObjectHandler::UnregisterGameObject(GameObject* gameObject)
{
	toDeleteObjects.push_back(gameObject->uniqueID);
	toDeleteNames.push_back(gameObject->name);
}


void GameObjectHandler::SetGameObjectActiveStatus(GameObject& gameObject, bool status)
{
	toToggleObjects.push(&gameObject);
	toToggleObjectsStatus.push(status);
}


void GameObjectHandler::TickHandler(Foxygine& foxygine)
{
	//Measures Time
	currentSystemTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	InvokeOnPreRender();
	//std::thread thread(UpdateThread, foxygine);
	Graphics::RenderFrame();


	//thread.join()
	InvokeOnPostRender();
	Keyboard::JoinInputThreads();
	Mouse::JoinInputThreads();

	long long updateTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	InvokeUpdate((float)lastUpdateDeltaTime, foxygine);
	lastUpdateDeltaTime = (double)(currentSystemTime - lastUpdateSystemTime) / 1000.0;
	
	//Fixed Update
	if (currentSystemTime - lastFixedUpdateSystemTime >= 19) {
		lastFixedUpdateDeltaTime = (currentSystemTime - lastFixedUpdateSystemTime) / 1000.0;
		lastFixedUpdateSystemTime = currentSystemTime;
		InvokeFixedUpdate((float)lastFixedUpdateDeltaTime, foxygine);
	}
	
	long long updateTimeEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//std::cout << "Update Time: " << (updateTimeEnd - updateTimeStart) << "ms" << std::endl;

	RegisterObjects();
	ToggleObjects();
	DeleteObjects();
	lastUpdateSystemTime = currentSystemTime;
}


void GameObjectHandler::InitHandler()
{
	currentSystemTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	RegisterObjects();
	ToggleObjects();
	DeleteObjects();

	for (auto go : gameObjectList) {
		go->Start();
	}

	lastUpdateSystemTime = currentSystemTime;
	lastFixedUpdateSystemTime = currentSystemTime;
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


void GameObjectHandler::InvokeUpdate(float deltaTime, Foxygine const & foxygine)
{
	Foxygine& foxy = const_cast<Foxygine&>(foxygine);
	foxy.UpdateFoxygine(deltaTime);

	//for (auto go : activeGameObjects) {
	//	go.second->Update(deltaTime);
	//}
	//std::cout << "pre update" << std::endl;

	std::for_each(std::execution::seq,
		gameObjectList.begin(),
		gameObjectList.end(),
		[](GameObject* go) {
			//std::cout << "updating: " << go->name << std::endl;
			go->Update(lastUpdateDeltaTime);
		});

	//std::cout << "update done" << std::endl << std::endl;
}


void GameObjectHandler::InvokeFixedUpdate(float deltaTime, Foxygine const & foxygine)
{
	Mouse::Tick(deltaTime);
	Foxygine& foxy = const_cast<Foxygine&>(foxygine);
	foxy.FixedUpdateFoxygine(deltaTime);

	//for (auto go : gameObjects) {
	//	go.second->FixedUpdate(deltaTime);
	//}

	std::for_each(std::execution::seq,
		gameObjectList.begin(),
		gameObjectList.end(),
		[](GameObject* go) {
			//std::cout << "updating: " << go->name << std::endl;
			go->FixedUpdate(lastFixedUpdateDeltaTime);
		});
}


void GameObjectHandler::UpdateThread(Foxygine const & foxygine)
{
	InvokeOnPreRender();
	InvokeOnPostRender();
	Keyboard::JoinInputThreads();
	Mouse::JoinInputThreads();
	InvokeUpdate((float)lastUpdateDeltaTime, foxygine);
	lastUpdateDeltaTime = (double)(currentSystemTime - lastUpdateSystemTime) / 1000.0;

	//Fixed Update
	if (currentSystemTime - lastFixedUpdateSystemTime >= 19) {
		lastFixedUpdateDeltaTime = (currentSystemTime - lastFixedUpdateSystemTime) / 1000.0;
		lastFixedUpdateSystemTime = currentSystemTime;
		InvokeFixedUpdate((float)lastFixedUpdateDeltaTime, foxygine);
	}

	DeleteObjects();
}
