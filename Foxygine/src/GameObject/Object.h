#pragma once
#include <string>


class Object
{
protected:
	bool isActive;

public:
	std::string name;

	void SetActive(bool);
	bool GetActiveSelf();
	virtual void OnEnable() {}
	virtual void OnDisable() {}
};

