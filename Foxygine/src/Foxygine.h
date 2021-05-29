#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <memory>


#define FOXYGINE_DEBUG
#define USE_NAME_LOOKUP
#define WIREFRAME_DEBUG
//#define USE_CACHE


class Foxygine
{
public:
	float r, g, b;

	Foxygine();
	~Foxygine();

	void StartFoxygine();
	void UpdateFoxygine(float deltaTime);
	void FixedUpdateFoxygine(float deltaTime);
	void EndFoxygine();


private:

};
