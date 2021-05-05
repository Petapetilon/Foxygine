#include "Mouse.h"
#include <iostream>



double Mouse::oldMousePosX;
double Mouse::oldMousePosY;

void Mouse::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	std::cout << xPos << ", " << yPos << std::endl;
}


void Mouse::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << xOffset << ", " << yOffset << std::endl;
}


void Mouse::SetupMouse()
{
	glfwSetCursorPosCallback(Window::GetInstance()->GLFW_GetWindow(), MouseMoveCallback);
	glfwSetScrollCallback(Window::GetInstance()->GLFW_GetWindow(), MouseScrollCallback);
}
