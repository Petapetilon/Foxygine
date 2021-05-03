#pragma once
#include "../Math/Vector2I.h"
#include "GLFW/glfw3.h"
#include <string>


class Window
{
private:
	double lastTime;
	double timeTillFPSDisplay;

	int windowWidth;
	int windowHeight;
	std::string windowName;
	GLFWwindow* GLFW_Window;

	static Window* instance;
	Window();

public:
	static Window* GetInstance();

	bool SetupWindow(Vector2I, std::string);
	GLFWwindow* GLFW_GetWindow();
	Vector2I GetWindowRersolution();
	void DisplayFPS();
};

