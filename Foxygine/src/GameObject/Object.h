#pragma once
#include <string>


class Object
{
protected:
	bool isActive;

public:
	std::string name;

	bool GetActiveSelf();
	virtual void SetActive(bool);
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnDestroy() {}
};

