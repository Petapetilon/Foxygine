#pragma once
#include "Window.h"
#include "GLFW//glfw3.h"
#include "../Math/Vector2.h"
#include <functional>
#include <thread>
#include <list>
#include <algorithm>


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

typedef std::function<void(Vector2)> TwoAxisFunc;
typedef std::function<void(ButtonCode, bool)> ClickNotifyFunc;


class Mouse
{
private:
	static double currentMousePosX;
	static double currentMousePosY;
	static double lastMousePosX;
	static double lastMousePosY;
	static double velocityX;
	static double velocityY;
	static double lastVelocityX;
	static double lastVelocityY;
	static double currentScrollX;
	static double currentScrollY;
	static int currentButton;
	static int currentAction;
	static bool mouseMoved;
	static bool mouseScrolled;
	static std::thread moveThread;
	static std::thread scrollThread;
	static std::thread buttonThread;
	static int latestMoveHandle;
	static int latestScrollHandle;
	static int latestButtonHandle;
	 
	static std::vector<std::tuple<TwoAxisFunc, int>> registered_Move_Functions;
	static std::vector<int> toUnregister_Move_Functions;
	 
	static std::vector<std::tuple<TwoAxisFunc, int>> registered_Scroll_Functions;
	static std::vector<int> toUnregister_Scroll_Functions;
	 
	static std::vector<std::tuple<ClickNotifyFunc, int>> registered_CNF_Functions;
	static std::vector<int> toUnregister_CNF_Functions;

	static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnteredCallback(GLFWwindow* window, int entered);
	static void SendMoveEvents();
	static void SendScrollEvents();
	static void SendButtonEvents();
	static void JoinMoveThread();
	static void JoinScrollThread();
	static void JoinButtonThread();

public:
	static void SetupMouse();
	static void FinishMouse();
	static Vector2 Position();
	static Vector2 Velocity();

	static int RegisterOnMove(const TwoAxisFunc);
	static void UnregisterOneMove(int);

	static int RegisterOnScroll(const TwoAxisFunc);
	static void UnregisterOnSCroll(int);

	static int RegisterOnButtonPress(const ClickNotifyFunc);
	static void UnregisterOnButtonPress(int);

	static void JoinInputThreads();
	static void Tick(float timeDelta);
};

