#include "Mouse.h"
#include <iostream>



double Mouse::currentMousePosX;
double Mouse::currentMousePosY;
double Mouse::lastMousePosX;
double Mouse::lastMousePosY;
double Mouse::velocityX;
double Mouse::velocityY;
double Mouse::currentScrollX;
double Mouse::currentScrollY;
int Mouse::currentButton;
int Mouse::currentAction;

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
	JoinMoveThread();
	lastMousePosX = currentMousePosX;
	lastMousePosY = currentMousePosY;
	currentMousePosX = xPos;
	currentMousePosY = yPos;

	SendMoveEvents();
}


void Mouse::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	JoinScrollThread();
	currentScrollX = xOffset;
	currentScrollY = yOffset;
	SendScrollEvents();
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	JoinButtonThread();
	currentButton = button;
	currentAction = action;
	SendButtonEvents();
}
 

void Mouse::SendMoveEvents()
{
	for (auto func : registered_Move_Functions) {
		func(Position());
	}
}


void Mouse::SendScrollEvents()
{
	for (auto func : registered_Scroll_Functions) {
		func(Vector2(currentScrollX, currentScrollY));
	}
}


void Mouse::SendButtonEvents()
{
	for (auto func : registered_SCF_Functions) {
		if ((int)func._Myfirst._Val == currentButton) {
			func._Get_rest()._Myfirst._Val(currentAction);
		}
	}

	for (auto func : registered_CNF_Functions) {
		func((ButtonCode)currentButton, currentAction);
	}
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
	glfwSetMouseButtonCallback(Window::GetInstance()->GLFW_GetWindow(), MouseButtonCallback);
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
	toUnregister_Scroll_Functions.push_back(func);
}


void Mouse::RegisterOnSpecificButtonPress(SpecificClickFunc func, ButtonCode code)
{
	JoinButtonThread();
	registered_SCF_Functions.push_back(std::tuple<ButtonCode, SpecificClickFunc>(code, func));
}


void Mouse::UnregisterOnSpecificButtonPress(SpecificClickFunc func, ButtonCode code)
{
	toUnregister_SCF_Functions.push_back(std::tuple<ButtonCode, SpecificClickFunc>(code, func));
}


void Mouse::RegisterOnButtonPress(ClickNotifyFunc func)
{
	JoinButtonThread();
	registered_CNF_Functions.push_back(func);
}


void Mouse::UnregisterOnButtonPress(ClickNotifyFunc func)
{
	toUnregister_CNF_Functions.push_back(func);
}


void Mouse::JoinInputThreads()
{
	JoinMoveThread();
	JoinScrollThread();
	JoinButtonThread();
}
