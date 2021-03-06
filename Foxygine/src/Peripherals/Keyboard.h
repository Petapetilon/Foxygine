#pragma once
#include "GLFW/glfw3.h"
#include <functional>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <algorithm>



enum class KeyCode {
	A,//=,//1,
	B,//=,//2,
	C,//=,//3,
	D,//=,//4,
	E,//=,//5,
	F,//=,//6,
	G,//=,//7,
	H,//=,//8,
	I,//=,//9,
	J,//=,//10,
	K,//=,//11,
	L,//=,//12,
	M,//=,//13,
	N,//=,//14,
	O,//=,//15,,//
	P,//=,//16,
	Q,//=,//17,
	R,//=,//18,
	S,//=,//19,
	T,//=,//20,
	U,//=,//21,
	V,//=,//22,
	W,//=,//23,
	X,//=,//24,
	Y,//=,//25,
	Z,//=,//26,
	Num_0,//=,//27,
	Num_1,//=,//28,
	Num_2,//=,//29,
	Num_3,//=,//30,
	Num_4,//=,//31,
	Num_5,//=,//32,
	Num_6,//=,//33,
	Num_7,//=,//34,
	Num_8,//=,//35,
	Num_9,//=,//36,
	NumPad_0,//=,//37,
	NumPad_1,//=,//38,
	NumPad_2,//=,//39,
	NumPad_3,//=,//40,
	NumPad_4,//=,//41,
	NumPad_5,//=,//42,
	NumPad_6,//=,//43,
	NumPad_7,//=,//44,
	NumPad_8,//=,//45,
	NumPad_9,//=,//46,
	NumPad_Period,//=,//47,
	NumPad_Enter,//=,//48,
	NumPad_Plus,//=,//49,
	NumPad_Minus,//=,//50,
	NumPad_Mul,//=,//51,
	NumPad_Div,//=,//52,
	F1,//=,//53,
	F2,//=,//54,
	F3,//=,//55,
	F4,//=,//56,
	F5,//=,//57,
	F6,//=,//58,
	F7,//=,//59,
	F8,//=,//60,
	F9,//=,//61,
	F10,//=,//62,
	F11,//=,//63,
	F12,//=,//64,
	L_Ctrl,//=,//65,
	L_Shift,//=,//66,
	L_Alt,//=,//67,
	Caps,//=,//68,
	Tab,//=,//69,
	Grave_Accent,//=,//70,
	Esc,//=,//71,
	R_Ctrl,//=,//72,
	R_Shift,//=,//73,
	R_Alt,//=,//74,
	Win,
	HashTag,
	Comma,
	Period,
	Slash,
	Semicolon,
	Minus,
	Equals,
	BackSpace,
	Enter,
	Space,
	Left_Bracket,
	Right_Bracket,
	Apostrophe,
	Arrow_Up,
	Arrow_Right,
	Arrow_Down,
	Arrow_Left,
	Delete,
	Insert,
	End,
	Pos1,
	Page_Up,
	Page_Down,
	Menu,
};

enum class KeyState {
	Up = 0,
	Down = 1,
	Held = 2
};


typedef std::function<void(KeyCode, KeyState)> GetKeyGetStateFunc;
typedef std::function<void(unsigned char)> TextInputCharFunc;


class Keyboard
{
private:
	static bool voidKeyPressCallback;
	static bool voidTextInputCallback;
	static int receivedScanCode;
	static int receivedAction;
	static unsigned int receivedTextInput;
	static KeyCode* keyCodeMapping;
	static int* scanCodeMapping;
	static int* glfwKeyMapping;
	static bool* currentKeyState;
	static std::thread keyInputThread;
	static std::thread textInputThread;
	static int latestKeyHandle;
	static int latestTextHandle;


	static std::vector<std::tuple<GetKeyGetStateFunc, int>> registered_GKGSF_Functions;
	static std::vector<int> toUnregister_GKGSF_Functions;

	static std::vector<std::tuple<TextInputCharFunc, int>> registered_TICF_Functions;
	static std::vector<int> toUnregister_TICF_Functions;

	static void KeyPressCallcack(GLFWwindow* widnow, int key, int scanCode, int action , int mods);
	static void TextInputCallback(GLFWwindow* window, unsigned int codePoint);
	static void SendKeyEvents();
	static void SendTextEvents();
	static void KeyPressJoinThread();
	static void TextInputJoinThread();

public:
	static void SetupKeyboard();

	static int RegisterOnAnyKeyCallback(GetKeyGetStateFunc callback);
	static void UnregisterOnAnyKeyCallback(int handle);

	static int RegisterOnTextInputCallback(TextInputCharFunc callback);
	static void UnregisterOnTextInputCallback(int handle);

	static bool GetKey(KeyCode keyCode);

	static void JoinInputThreads();
};

