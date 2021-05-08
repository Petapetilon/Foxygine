#pragma once
#include "../Math/Vector2I.h"
#include "GLFW/glfw3.h"
//#include "../Graphics/Graphics.h"
#include <string>
#include <functional>


typedef std::function<void(int, int)> ResizeFunc;

class Window
{
private:
	double lastTime;
	double timeTillFPSDisplay;

	int windowWidth;
	int windowHeight;
	std::string windowName;
	GLFWwindow* GLFW_Window;
	ResizeFunc resizeCallee;

	static Window* instance;
	
	Window();
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

public:
	static Window* GetInstance();

	bool SetupWindow(Vector2I, std::string);
	GLFWwindow* GLFW_GetWindow();
	Vector2I GetWindowResolution();
	void SetWindowResizeCallback(ResizeFunc callback);
	void DisplayFPS();
};

