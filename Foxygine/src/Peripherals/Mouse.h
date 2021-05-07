#pragma once
#include "Window.h"
#include "GLFW//glfw3.h"
#include "../Math/Vector2.h"
#include <functional>
#include <thread>
#include <list>


enum class ButtonCode {
	Left = 0,
	Right = 1,
	Middle = 2,
	Custom_1 = 3,
	Custom_2 = 4,
	Custom_3 = 5,
	Custom_4 = 6,
	Custom_5 = 7,
	Custom_6 = 8,
	Custom_7 = 9,
	Custom_8 = 10,
	Custom_9 = 11,
	Custom_10 = 12,
	Custom_11 = 13,
	Custom_12 = 14,
	Custom_13 = 15,
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
	static double currentScrollX;
	static double currentScrollY;
	static int currentButton;
	static int currentAction;

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
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
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

