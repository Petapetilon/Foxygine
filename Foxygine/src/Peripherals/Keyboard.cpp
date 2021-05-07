#include "Keyboard.h"
#include "Window.h"
#include <iostream>


bool Keyboard::voidKeyPressCallback;
bool Keyboard::voidTextInputCallback;
int Keyboard::receivedScanCode;
int Keyboard::receivedAction;
unsigned int Keyboard::receivedTextInput;
KeyCode* Keyboard::keyCodeMapping;
std::thread Keyboard::keyInputThread;
std::thread Keyboard::textInputThread;

std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::registered_KNF_D_Functions;
std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::toUnregister_KNF_D_Functions;

std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::registered_KNF_H_Functions;
std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::toUnregister_KNF_H_Functions;

std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::registered_KNF_U_Functions;
std::list<std::tuple<KeyCode, KeyNotifyFunc>> Keyboard::toUnregister_KNF_U_Functions;

std::list<std::tuple<KeyCode, KeyStateFunc>> Keyboard::registered_KSF_Functions;
std::list<std::tuple<KeyCode, KeyStateFunc>> Keyboard::toUnregister_KSF_Functions;

std::list<GetKeyGetStateFunc> Keyboard::registered_GKGSF_Functions;
std::list<GetKeyGetStateFunc> Keyboard::toUnregister_GKGSF_Functions;

std::list<TextInputCharFunc> Keyboard::registered_TICF_Functions;
std::list<TextInputCharFunc> Keyboard::toUnregister_TICF_Functions;



void Keyboard::SetupKeyboard()
{
	keyCodeMapping = new KeyCode[512];
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_A)] = KeyCode::A;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_B)] = KeyCode::B;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_C)] = KeyCode::C;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_D)] = KeyCode::D;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_E)] = KeyCode::E;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F)] = KeyCode::F;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_G)] = KeyCode::G;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_H)] = KeyCode::H;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_I)] = KeyCode::I;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_J)] = KeyCode::J;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_K)] = KeyCode::K;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_L)] = KeyCode::L;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_M)] = KeyCode::M;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_N)] = KeyCode::N;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_O)] = KeyCode::O;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_P)] = KeyCode::P;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_Q)] = KeyCode::Q;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_R)] = KeyCode::R;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_S)] = KeyCode::S;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_T)] = KeyCode::T;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_U)] = KeyCode::U;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_V)] = KeyCode::V;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_W)] = KeyCode::W;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_X)] = KeyCode::X;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_Y)] = KeyCode::Y;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_Z)] = KeyCode::Z;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_0)] = KeyCode::Num_0;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_1)] = KeyCode::Num_1;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_2)] = KeyCode::Num_2;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_3)] = KeyCode::Num_3;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_4)] = KeyCode::Num_4;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_5)] = KeyCode::Num_5;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_6)] = KeyCode::Num_6;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_7)] = KeyCode::Num_7;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_8)] = KeyCode::Num_8;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_9)] = KeyCode::Num_9;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_0)] = KeyCode::NumPad_0;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_1)] = KeyCode::NumPad_1;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_2)] = KeyCode::NumPad_2;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_3)] = KeyCode::NumPad_3;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_4)] = KeyCode::NumPad_4;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_5)] = KeyCode::NumPad_5;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_6)] = KeyCode::NumPad_6;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_7)] = KeyCode::NumPad_7;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_8)] = KeyCode::NumPad_8;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_9)] = KeyCode::NumPad_9;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_DECIMAL)] = KeyCode::NumPad_Period;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_ENTER)] = KeyCode::NumPad_Enter;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_ADD)] = KeyCode::NumPad_Plus;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_SUBTRACT)] = KeyCode::NumPad_Minus;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_MULTIPLY)] = KeyCode::NumPad_Mul;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_KP_DIVIDE)] = KeyCode::NumPad_Div;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F1)] = KeyCode::F1;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F2)] = KeyCode::F2;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F3)] = KeyCode::F3;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F4)] = KeyCode::F4;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F5)] = KeyCode::F5;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F6)] = KeyCode::F6;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F7)] = KeyCode::F7;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F8)] = KeyCode::F8;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F9)] = KeyCode::F9;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F10)] = KeyCode::F10;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F11)] = KeyCode::F11;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_F12)] = KeyCode::F12;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_LEFT_CONTROL)] = KeyCode::L_Ctrl;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_LEFT_SHIFT)] = KeyCode::L_Shift;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_LEFT_ALT)] = KeyCode::L_Alt;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_CAPS_LOCK)] = KeyCode::Caps;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_TAB)] = KeyCode::Tab;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_GRAVE_ACCENT)] = KeyCode::Grave_Accent;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_ESCAPE)] = KeyCode::Esc;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_RIGHT_CONTROL)] = KeyCode::R_Ctrl;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_RIGHT_SHIFT)] = KeyCode::R_Shift;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_RIGHT_ALT)] = KeyCode::R_Alt;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_HOME)] = KeyCode::Win;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_UNKNOWN)] = KeyCode::HashTag;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_COMMA)] = KeyCode::Comma;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_PERIOD)] = KeyCode::Period;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_SLASH)] = KeyCode::Slash;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_SEMICOLON)] = KeyCode::Semicolon;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_MINUS)] = KeyCode::Minus;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_EQUAL)] = KeyCode::Equals;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_BACKSPACE)] = KeyCode::BackSpace;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_ENTER)] = KeyCode::Enter;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_SPACE)] = KeyCode::Space;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_LEFT_BRACKET)] = KeyCode::Left_Bracket;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_RIGHT_BRACKET)] = KeyCode::Right_Bracket;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_APOSTROPHE)] = KeyCode::Apostrophe;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_UP)] = KeyCode::Arrow_Up;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_RIGHT)] = KeyCode::Arrow_Right;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_DOWN)] = KeyCode::Arrow_Down;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_LEFT)] = KeyCode::Arrow_Left;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_DELETE)] = KeyCode::Delete;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_INSERT)] = KeyCode::Insert;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_END)] = KeyCode::End;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_PAUSE)] = KeyCode::Pos1;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_PAGE_UP)] = KeyCode::Page_Up;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_PAGE_DOWN)] = KeyCode::Page_Down;
	keyCodeMapping[(unsigned int)glfwGetKeyScancode(GLFW_KEY_MENU)] = KeyCode::Menu;

	glfwSetKeyCallback(Window::GetInstance()->GLFW_GetWindow(), KeyPressCallcack);
	glfwSetCharCallback(Window::GetInstance()->GLFW_GetWindow(), TextInputCallback);
}


void Keyboard::SendKeyEvents()
{
	if (receivedAction == GLFW_PRESS) {
		for (auto func : registered_KNF_D_Functions) {
			if (func._Myfirst._Val == keyCodeMapping[receivedScanCode]) {
				func._Get_rest()._Myfirst._Val();
			}
		}
	}
	else if (receivedAction == GLFW_REPEAT) {
		for (auto func : registered_KNF_H_Functions) {
			if (func._Myfirst._Val == keyCodeMapping[receivedScanCode]) {
				func._Get_rest()._Myfirst._Val();
			}
		}
	}
	else {
		for (auto func : registered_KNF_U_Functions) {
			if (func._Myfirst._Val == keyCodeMapping[receivedScanCode]) {
				func._Get_rest()._Myfirst._Val();
			}
		}
	}

	for (auto func : registered_KSF_Functions) {
		if (func._Myfirst._Val == keyCodeMapping[receivedScanCode]) {
			func._Get_rest()._Myfirst._Val((KeyState)receivedAction);
		}
	}


	for (auto func : registered_GKGSF_Functions) {
		func(keyCodeMapping[receivedScanCode], (KeyState)receivedAction);
	}
}


void Keyboard::SendTextEvents()
{
	for (auto func : registered_TICF_Functions) {
		func(receivedTextInput);
	}
}


void Keyboard::KeyPressJoinThread()
{
	if (keyInputThread.joinable()) {
		keyInputThread.join();
	}


	for (auto func : toUnregister_KNF_D_Functions) {
		registered_KNF_D_Functions.remove(func);
	}

	toUnregister_KNF_D_Functions.clear();


	for (auto func : toUnregister_KNF_H_Functions) {
		registered_KNF_H_Functions.remove(func);
	}

	toUnregister_KNF_H_Functions.clear();


	for (auto func : toUnregister_KNF_U_Functions) {
		registered_KNF_U_Functions.remove(func);
	}

	toUnregister_KNF_U_Functions.clear();


	for (auto func : toUnregister_KSF_Functions) {
		registered_KSF_Functions.remove(func);
	}

	toUnregister_KSF_Functions.clear();


	for (auto func : toUnregister_GKGSF_Functions) {
		registered_GKGSF_Functions.remove(func);
	}

	toUnregister_GKGSF_Functions.clear();
}


void Keyboard::TextInputJoinThread() {
	if (textInputThread.joinable()) {
		textInputThread.join();
	}

	for (auto func : toUnregister_TICF_Functions) {
		registered_TICF_Functions.remove(func);
	}

	toUnregister_TICF_Functions.clear();
}





void Keyboard::RegisterOnKeyDownStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	KeyPressJoinThread();
	registered_KNF_D_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}

void Keyboard::UnregisterOnKeyDownStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	toUnregister_KNF_D_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}
	 


void Keyboard::RegisterOnKeyHeldStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	KeyPressJoinThread();
	registered_KNF_H_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}

void Keyboard::UnregisterOnKeyHeldStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	toUnregister_KNF_H_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}



void Keyboard::RegisterOnKeyUpStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	KeyPressJoinThread();
	registered_KNF_U_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}

void Keyboard::UnregisterOnKeyUpStateCallback(KeyNotifyFunc callback, KeyCode keyCode)
{
	toUnregister_KNF_U_Functions.push_back(std::tuple<KeyCode, KeyNotifyFunc>(keyCode, callback));
}
	 


void Keyboard::RegisterOnKeyAnyStateCallback(KeyStateFunc callback, KeyCode keyCode)
{
	KeyPressJoinThread();
	registered_KSF_Functions.push_back(std::tuple<KeyCode, KeyStateFunc>(keyCode, callback));
}

void Keyboard::UnregisterOnKeyAnyStateCallback(KeyStateFunc callback, KeyCode keyCode)
{
	registered_KSF_Functions.push_back(std::tuple<KeyCode, KeyStateFunc>(keyCode, callback));
}



void Keyboard::RegisterOnAnyKeyCallback(GetKeyGetStateFunc callback)
{
	KeyPressJoinThread();
	registered_GKGSF_Functions.push_back(callback);
}

void Keyboard::UnregisterOnAnyKeyCallback(GetKeyGetStateFunc callback)
{
	toUnregister_GKGSF_Functions.push_back(callback);
}



void Keyboard::KeyPressCallcack(GLFWwindow* window, int key, int scanCode, int action, int mods)
{	
	if (!voidKeyPressCallback) {
		receivedScanCode = scanCode;
		receivedAction = action;
		KeyPressJoinThread();
		keyInputThread = std::thread(SendKeyEvents);
	}
}


void Keyboard::TextInputCallback(GLFWwindow* window, unsigned int codePoint)
{
	if (!voidTextInputCallback) {
		receivedTextInput = codePoint;
		TextInputJoinThread();
		textInputThread = std::thread(SendTextEvents);
	}
}


void Keyboard::RegisterOnTextInputCallback(TextInputCharFunc callback) 
{
	TextInputJoinThread();
	registered_TICF_Functions.push_back(callback);
}


void Keyboard::UnregisterOnTextInputCallback(TextInputCharFunc callback) 
{
	toUnregister_TICF_Functions.push_back(callback);
}


void Keyboard::JoinInputThreads() {
	KeyPressJoinThread();
	TextInputJoinThread();	
}
