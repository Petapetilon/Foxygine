#include "Mouse.h"
#include <iostream>



double Mouse::currentMousePosX;
double Mouse::currentMousePosY;
double Mouse::lastMousePosX;
double Mouse::lastMousePosY;
double Mouse::velocityX;
double Mouse::velocityY;
double Mouse::lastVelocityX;
double Mouse::lastVelocityY;
double Mouse::currentScrollX;
double Mouse::currentScrollY;
int Mouse::currentButton;
int Mouse::currentAction;
bool Mouse::mouseMoved;
bool Mouse::mouseScrolled;
std::thread Mouse::moveThread;
std::thread Mouse::scrollThread;
std::thread Mouse::buttonThread;
int Mouse::latestMoveHandle;
int Mouse::latestScrollHandle;
int Mouse::latestButtonHandle;

std::vector<std::tuple<TwoAxisFunc, int>> Mouse::registered_Move_Functions;
std::vector<int> Mouse::toUnregister_Move_Functions;
			
std::vector<std::tuple<TwoAxisFunc, int>> Mouse::registered_Scroll_Functions;
std::vector<int> Mouse::toUnregister_Scroll_Functions;
			
std::vector<std::tuple<ClickNotifyFunc, int>> Mouse::registered_CNF_Functions;
std::vector<int> Mouse::toUnregister_CNF_Functions;


void Mouse::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	JoinMoveThread();
	lastMousePosX = currentMousePosX;
	lastMousePosY = currentMousePosY;
	currentMousePosX = xPos;
	currentMousePosY = yPos;
	//SendMoveEvents();

	mouseMoved = true;
	auto vel = Velocity();
	lastVelocityX = vel.x;
	lastVelocityY = vel.y;
}


void Mouse::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	JoinScrollThread();
	currentScrollX = xOffset;
	currentScrollY = yOffset;
	//SendScrollEvents();

	mouseScrolled = true;
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	JoinButtonThread();
	currentButton = button;
	currentAction = action;
	//SendButtonEvents();
}

void Mouse::MouseEnteredCallback(GLFWwindow* window, int entered)
{
	if (entered) {
		lastMousePosX = currentMousePosX;
		lastMousePosY = currentMousePosY;
	}
}
 

void Mouse::SendMoveEvents()
{
	for (auto func : registered_Move_Functions) {
		func._Myfirst._Val(Position());
	}
}


void Mouse::SendScrollEvents()
{
	for (auto func : registered_Scroll_Functions) {
		func._Myfirst._Val(Vector2(currentScrollX, currentScrollY));
	}
}


void Mouse::SendButtonEvents()
{
	for (auto func : registered_CNF_Functions) {
		func._Myfirst._Val((ButtonCode)currentButton, currentAction);
	}
}


void Mouse::JoinMoveThread()
{
	if (moveThread.joinable()) {
		moveThread.join();
	}

	auto funcIt = std::begin(registered_Move_Functions);
	while (funcIt != registered_Move_Functions.begin()) {
		for (auto delFunc : toUnregister_Move_Functions) {
			if (funcIt->_Get_rest()._Myfirst._Val == delFunc) {
				funcIt = registered_Move_Functions.erase(funcIt);
			}
			else {
				++funcIt;
			}
		}
	}

	toUnregister_Move_Functions.clear();
}


void Mouse::JoinScrollThread()
{
	if (scrollThread.joinable()) {
		scrollThread.join();
	}

	auto funcIt = std::begin(registered_Scroll_Functions);
	while (funcIt != registered_Scroll_Functions.begin()) {
		for (auto delFunc : toUnregister_Scroll_Functions) {
			if (funcIt->_Get_rest()._Myfirst._Val == delFunc) {
				funcIt = registered_Scroll_Functions.erase(funcIt);
			}
			else {
				++funcIt;
			}
		}
	}

	toUnregister_Scroll_Functions.clear();
}


void Mouse::JoinButtonThread()
{
	if (buttonThread.joinable()) {
		buttonThread.join();
	}

	auto funcIt = std::begin(registered_CNF_Functions);
	while (funcIt != registered_CNF_Functions.begin()) {
		for (auto delFunc : toUnregister_CNF_Functions) {
			if (funcIt->_Get_rest()._Myfirst._Val == delFunc) {
				funcIt = registered_CNF_Functions.erase(funcIt);
			}
			else {
				++funcIt;
			}
		}
	}

	toUnregister_CNF_Functions.clear();
}


void Mouse::SetupMouse()
{
	glfwSetCursorPosCallback(Window::GetInstance()->GLFW_GetWindow(), MouseMoveCallback);
	glfwSetScrollCallback(Window::GetInstance()->GLFW_GetWindow(), MouseScrollCallback);
	glfwSetMouseButtonCallback(Window::GetInstance()->GLFW_GetWindow(), MouseButtonCallback);
	glfwSetCursorEnterCallback(Window::GetInstance()->GLFW_GetWindow(), MouseEnteredCallback);
	glfwSetInputMode(Window::GetInstance()->GLFW_GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Mouse::FinishMouse()
{
	glfwSetInputMode(Window::GetInstance()->GLFW_GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


Vector2 Mouse::Position()
{
	return Vector2((float)currentMousePosX, (float)currentMousePosY);
}


Vector2 Mouse::Velocity()
{
	return Vector2((float)(currentMousePosX - lastMousePosX), (float)(currentMousePosY - lastMousePosY));
}


int Mouse::RegisterOnMove(const TwoAxisFunc func)
{
	JoinMoveThread();
	registered_Move_Functions.push_back(std::tuple<TwoAxisFunc, int>(func, ++latestMoveHandle));
	return latestMoveHandle;
}


void Mouse::UnregisterOneMove(int handle)
{
	toUnregister_Move_Functions.push_back(handle);
}


int Mouse::RegisterOnScroll(const TwoAxisFunc func)
{
	JoinScrollThread();
	registered_Scroll_Functions.push_back(std::tuple<TwoAxisFunc, int>(func, ++latestScrollHandle));
	return latestScrollHandle;
}


void Mouse::UnregisterOnSCroll(int handle)
{
	toUnregister_Scroll_Functions.push_back(handle);
}


int Mouse::RegisterOnButtonPress(const ClickNotifyFunc func)
{
	JoinButtonThread();
	registered_CNF_Functions.push_back(std::tuple<ClickNotifyFunc, int>(func, ++latestButtonHandle));
	return latestButtonHandle;
}


void Mouse::UnregisterOnButtonPress(int handle)
{
	toUnregister_CNF_Functions.push_back(handle);
}


void Mouse::JoinInputThreads()
{
	JoinMoveThread();
	JoinScrollThread();
	JoinButtonThread();
}

void Mouse::Tick(float timeDelta)
{
	if (mouseMoved) {
		auto vel = Velocity();
		if (lastVelocityX == vel.x && lastVelocityY == vel.y) {
			lastMousePosX = currentMousePosX;
			lastMousePosY = currentMousePosY;
			mouseMoved = false;
		}
	}
}
