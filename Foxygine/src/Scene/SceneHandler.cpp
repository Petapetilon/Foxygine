#include "SceneHandler.h"

std::shared_ptr<Scene> SceneHandler::CreateScene(std::string _name)
{
	return std::shared_ptr<Scene>();
}

void SceneHandler::DeleteScene(std::string _name)
{
}

bool SceneHandler::LoadScene(std::string _name)
{
	return false;
}

bool SceneHandler::UnloadScene(std::string _name)
{
	return false;
}

void SceneHandler::EnableScene(std::string _name)
{
}

void SceneHandler::DisableScene(std::string _name)
{
}

bool SceneHandler::SetActiveScene(std::shared_ptr<Scene> _scene)
{
	return false;
}
