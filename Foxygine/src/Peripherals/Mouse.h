#pragma once
#include "Window.h"
#include "GLFW//glfw3.h"
#include "../Math/Vector2.h"
#include <functional>
#include <thread>
#include <list>


enum class ButtonCode {
	Left,
	Right,
	Middle,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15
};


typedef void(*TwoAxisFunc)(Vector2);
typedef void(*SpecificClickFunc)(bool);
typedef void(*ClickNotifyFunc)(ButtonCode, bool);


class Mouse
{
private:
	static double currentMousePosX;
	static double currentMousePosY;
	static double lastMousePosX;
	static double lastMousePosY;
	static double velocityX;
	static double velocityY;

	static std::thread moveThread;
	static std::thread scrollThread;
	static std::thread buttonThread;
	 
	static std::list<std::tuple<ButtonCode, SpecificClickFunc>> registered_SCF_Functions;
	static std::list<std::tuple<ButtonCode, SpecificClickFunc>> toUnregister_SCF_Functions;
	 
	static std::list<TwoAxisFunc> registered_Move_Functions;
	static std::list<TwoAxisFunc> toUnregister_Move_Functions;
	 
	static std::list<TwoAxisFunc> registered_Scroll_Functions;
	static std::list<TwoAxisFunc> toUnregister_Scroll_Functions;
	 
	static std::list<ClickNotifyFunc> registered_CNF_Functions;
	static std::list<ClickNotifyFunc> toUnregister_CNF_Functions;

	static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void SendMoveEvents();
	static void SendScrollEvents();
	static void SendButtonEvents();
	static void JoinMoveThread();
	static void JoinScrollThread();
	static void JoinButtonThread();

public:
	static void SetupMouse();
	static Vector2 Position();
	static Vector2 Velocity();

	static void RegisterOnMove(TwoAxisFunc);
	static void UnregisterOneMove(TwoAxisFunc);

	static void RegisterOnScroll(TwoAxisFunc);
	static void UnregisterOnSCroll(TwoAxisFunc);

	static void RegisterOnSpecificButtonPress(SpecificClickFunc, ButtonCode);
	static void UnregisterOnSpecificButtonPress(SpecificClickFunc, ButtonCode);

	static void RegisterOnButtonPress(ClickNotifyFunc);
	static void UnregisterOnButtonPress(ClickNotifyFunc);

	static void JoinInputThreads();
};

