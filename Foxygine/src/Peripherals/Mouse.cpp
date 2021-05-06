#include "Mouse.h"
#include <iostream>



double Mouse::currentMousePosX;
double Mouse::currentMousePosY;
double Mouse::lastMousePosX;
double Mouse::lastMousePosY;
double Mouse::velocityX;
double Mouse::velocityY;

std::thread Mouse::moveThread;
std::thread Mouse::scrollThread;
std::thread Mouse::buttonThread;

std::list<std::tuple<ButtonCode, SpecificClickFunc>> Mouse::registered_SCF_Functions;
std::list<std::tuple<ButtonCode, SpecificClickFunc>> Mouse::toUnregister_SCF_Functions;

std::list<TwoAxisFunc> Mouse::registered_Move_Functions;
std::list<TwoAxisFunc> Mouse::toUnregister_Move_Functions;

std::list<TwoAxisFunc> Mouse::registered_Scroll_Functions;
std::list<TwoAxisFunc> Mouse::toUnregister_Scroll_Functions;

std::list<ClickNotifyFunc> Mouse::registered_CNF_Functions;
std::list<ClickNotifyFunc> Mouse::toUnregister_CNF_Functions;


void Mouse::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	//std::cout << xPos << ", " << yPos << std::endl;
	lastMousePosX = currentMousePosX;
	lastMousePosY = currentMousePosY;
	currentMousePosX = xPos;
	currentMousePosY = yPos;

	//auto vel = Velocity();
	//std::cout << vel.x << ", " << vel.y << std::endl;
}


void Mouse::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << xOffset << ", " << yOffset << std::endl;
}
 

void Mouse::SendMoveEvents()
{

}


void Mouse::SendScrollEvents()
{
}


void Mouse::SendButtonEvents()
{

}


void Mouse::JoinMoveThread()
{
	if (moveThread.joinable()) {
		moveThread.join();
	}

	for (auto func : toUnregister_Move_Functions) {
		registered_Move_Functions.remove(func);
	}

	toUnregister_Move_Functions.clear();
}


void Mouse::JoinScrollThread()
{
	if (scrollThread.joinable()) {
		scrollThread.join();
	}

	for (auto func : toUnregister_Scroll_Functions) {
		registered_Scroll_Functions.remove(func);
	}

	toUnregister_Scroll_Functions.clear();
}


void Mouse::JoinButtonThread()
{
	if (buttonThread.joinable()) {
		buttonThread.join();
	}

	for (auto func : toUnregister_CNF_Functions) {
		registered_CNF_Functions.remove(func);
	}

	toUnregister_CNF_Functions.clear();


	for (auto func : toUnregister_SCF_Functions) {
		registered_SCF_Functions.remove(func);
	}

	toUnregister_SCF_Functions.clear();
}




void Mouse::SetupMouse()
{
	glfwSetCursorPosCallback(Window::GetInstance()->GLFW_GetWindow(), MouseMoveCallback);
	glfwSetScrollCallback(Window::GetInstance()->GLFW_GetWindow(), MouseScrollCallback);
}


Vector2 Mouse::Position()
{
	return Vector2((float)currentMousePosX, (float)currentMousePosY);
}


Vector2 Mouse::Velocity()
{
	return Vector2((float)(currentMousePosX - lastMousePosX), (float)(currentMousePosY - lastMousePosY));
}


void Mouse::RegisterOnMove(TwoAxisFunc func)
{
	JoinMoveThread();
	registered_Move_Functions.push_back(func);
}


void Mouse::UnregisterOneMove(TwoAxisFunc func)
{
	toUnregister_Move_Functions.push_back(func);
}


void Mouse::RegisterOnScroll(TwoAxisFunc func)
{
	JoinScrollThread();
	registered_Scroll_Functions.push_back(func);
}


void Mouse::UnregisterOnSCroll(TwoAxisFunc func)
{
}


void Mouse::RegisterOnSpecificButtonPress(SpecificClickFunc, ButtonCode)
{
}


void Mouse::UnregisterOnSpecificButtonPress(SpecificClickFunc, ButtonCode)
{
}


void Mouse::RegisterOnButtonPress(ClickNotifyFunc)
{
}


void Mouse::UnregisterOnButtonPress(ClickNotifyFunc)
{
}


void Mouse::JoinInputThreads()
{
}
