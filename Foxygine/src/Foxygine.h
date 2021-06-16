#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <memory>




#define FOXYGINE_DEBUG
#define USE_NAME_LOOKUP
//#define WIREFRAME_DEBUG
//#define USE_CACHE

class GameObject;


class Foxygine
{
public:
	float r, g, b;
	std::shared_ptr<GameObject> bullet;
	std::shared_ptr<GameObject> player;

	Foxygine();
	~Foxygine();

	void StartFoxygine();
	void UpdateFoxygine(float deltaTime);
	void FixedUpdateFoxygine(float deltaTime);
	void EndFoxygine();



private:

};
