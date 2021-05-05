#pragma once
#include "Window.h"
#include "GLFW//glfw3.h"


class Mouse
{
private:
	static double oldMousePosX;
	static double oldMousePosY;

	static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

public:
	static void SetupMouse();
};

