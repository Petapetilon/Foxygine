#pragma once
#include <string>
#include <memory>
#include "Scene.h"


class SceneHandler
{
public:
	std::shared_ptr<Scene> CreateScene(std::string _name);
	void DeleteScene(std::string _name);

	bool LoadScene(std::string _name);
	bool UnloadScene(std::string _name);

	void EnableScene(std::string _name);
	void DisableScene(std::string _name);

	bool SetActiveScene(std::shared_ptr<Scene> _scene);
};

