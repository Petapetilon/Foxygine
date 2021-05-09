#include "Keyboard.h"
#include "Window.h"
#include <iostream>


bool Keyboard::voidKeyPressCallback;
bool Keyboard::voidTextInputCallback;
int Keyboard::receivedScanCode;
int Keyboard::receivedAction;
unsigned int Keyboard::receivedTextInput;
KeyCode* Keyboard::keyCodeMapping;
int* Keyboard::scanCodeMapping;
int* Keyboard::glfwKeyMapping;
bool* Keyboard::currentKeyState;
std::thread Keyboard::keyInputThread;
std::thread Keyboard::textInputThread;
int Keyboard::latestKeyHandle;
int Keyboard::latestTextHandle;


std::vector<std::tuple<GetKeyGetStateFunc, int>> Keyboard::registered_GKGSF_Functions;
std::vector<int> Keyboard::toUnregister_GKGSF_Functions;
		
std::vector<std::tuple<TextInputCharFunc, int>> Keyboard::registered_TICF_Functions;
std::vector<int> Keyboard::toUnregister_TICF_Functions;



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

	scanCodeMapping = new int[128];
	scanCodeMapping[(unsigned int)KeyCode::A] = glfwGetKeyScancode(GLFW_KEY_A);
	scanCodeMapping[(unsigned int)KeyCode::B] = glfwGetKeyScancode(GLFW_KEY_B);
	scanCodeMapping[(unsigned int)KeyCode::C] = glfwGetKeyScancode(GLFW_KEY_C);
	scanCodeMapping[(unsigned int)KeyCode::D] = glfwGetKeyScancode(GLFW_KEY_D);
	scanCodeMapping[(unsigned int)KeyCode::E] = glfwGetKeyScancode(GLFW_KEY_E);
	scanCodeMapping[(unsigned int)KeyCode::F] = glfwGetKeyScancode(GLFW_KEY_F);
	scanCodeMapping[(unsigned int)KeyCode::G] = glfwGetKeyScancode(GLFW_KEY_G);
	scanCodeMapping[(unsigned int)KeyCode::H] = glfwGetKeyScancode(GLFW_KEY_H);
	scanCodeMapping[(unsigned int)KeyCode::I] = glfwGetKeyScancode(GLFW_KEY_I);
	scanCodeMapping[(unsigned int)KeyCode::J] = glfwGetKeyScancode(GLFW_KEY_J);
	scanCodeMapping[(unsigned int)KeyCode::K] = glfwGetKeyScancode(GLFW_KEY_K);
	scanCodeMapping[(unsigned int)KeyCode::L] = glfwGetKeyScancode(GLFW_KEY_L);
	scanCodeMapping[(unsigned int)KeyCode::M] = glfwGetKeyScancode(GLFW_KEY_M);
	scanCodeMapping[(unsigned int)KeyCode::N] = glfwGetKeyScancode(GLFW_KEY_N);
	scanCodeMapping[(unsigned int)KeyCode::O] = glfwGetKeyScancode(GLFW_KEY_O);
	scanCodeMapping[(unsigned int)KeyCode::P] = glfwGetKeyScancode(GLFW_KEY_P);
	scanCodeMapping[(unsigned int)KeyCode::Q] = glfwGetKeyScancode(GLFW_KEY_Q);
	scanCodeMapping[(unsigned int)KeyCode::R] = glfwGetKeyScancode(GLFW_KEY_R);
	scanCodeMapping[(unsigned int)KeyCode::S] = glfwGetKeyScancode(GLFW_KEY_S);
	scanCodeMapping[(unsigned int)KeyCode::T] = glfwGetKeyScancode(GLFW_KEY_T);
	scanCodeMapping[(unsigned int)KeyCode::U] = glfwGetKeyScancode(GLFW_KEY_U);
	scanCodeMapping[(unsigned int)KeyCode::V] = glfwGetKeyScancode(GLFW_KEY_V);
	scanCodeMapping[(unsigned int)KeyCode::W] = glfwGetKeyScancode(GLFW_KEY_W);
	scanCodeMapping[(unsigned int)KeyCode::X] = glfwGetKeyScancode(GLFW_KEY_X);
	scanCodeMapping[(unsigned int)KeyCode::Y] = glfwGetKeyScancode(GLFW_KEY_Y);
	scanCodeMapping[(unsigned int)KeyCode::Z] = glfwGetKeyScancode(GLFW_KEY_Z);
	scanCodeMapping[(unsigned int)KeyCode::Num_0] = glfwGetKeyScancode(GLFW_KEY_0);
	scanCodeMapping[(unsigned int)KeyCode::Num_1] = glfwGetKeyScancode(GLFW_KEY_1);
	scanCodeMapping[(unsigned int)KeyCode::Num_2] = glfwGetKeyScancode(GLFW_KEY_2);
	scanCodeMapping[(unsigned int)KeyCode::Num_3] = glfwGetKeyScancode(GLFW_KEY_3);
	scanCodeMapping[(unsigned int)KeyCode::Num_4] = glfwGetKeyScancode(GLFW_KEY_4);
	scanCodeMapping[(unsigned int)KeyCode::Num_5] = glfwGetKeyScancode(GLFW_KEY_5);
	scanCodeMapping[(unsigned int)KeyCode::Num_6] = glfwGetKeyScancode(GLFW_KEY_6);
	scanCodeMapping[(unsigned int)KeyCode::Num_7] = glfwGetKeyScancode(GLFW_KEY_7);
	scanCodeMapping[(unsigned int)KeyCode::Num_8] = glfwGetKeyScancode(GLFW_KEY_8);
	scanCodeMapping[(unsigned int)KeyCode::Num_9] = glfwGetKeyScancode(GLFW_KEY_9);
	scanCodeMapping[(unsigned int)KeyCode::NumPad_0] = glfwGetKeyScancode(GLFW_KEY_KP_0);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_1] = glfwGetKeyScancode(GLFW_KEY_KP_1);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_2] = glfwGetKeyScancode(GLFW_KEY_KP_2);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_3] = glfwGetKeyScancode(GLFW_KEY_KP_3);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_4] = glfwGetKeyScancode(GLFW_KEY_KP_4);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_5] = glfwGetKeyScancode(GLFW_KEY_KP_5);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_6] = glfwGetKeyScancode(GLFW_KEY_KP_6);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_7] = glfwGetKeyScancode(GLFW_KEY_KP_7);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_8] = glfwGetKeyScancode(GLFW_KEY_KP_8);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_9] = glfwGetKeyScancode(GLFW_KEY_KP_9);				   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Period] = glfwGetKeyScancode(GLFW_KEY_KP_DECIMAL);	   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Enter] = glfwGetKeyScancode(GLFW_KEY_KP_ENTER);		   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Plus] = glfwGetKeyScancode(GLFW_KEY_KP_ADD);			   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Minus] = glfwGetKeyScancode(GLFW_KEY_KP_SUBTRACT);	   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Mul] = glfwGetKeyScancode(GLFW_KEY_KP_MULTIPLY);		   
	scanCodeMapping[(unsigned int)KeyCode::NumPad_Div] = glfwGetKeyScancode(GLFW_KEY_KP_DIVIDE);		   
	scanCodeMapping[(unsigned int)KeyCode::F1] = glfwGetKeyScancode(GLFW_KEY_F1);						   
	scanCodeMapping[(unsigned int)KeyCode::F2] = glfwGetKeyScancode(GLFW_KEY_F2);						   
	scanCodeMapping[(unsigned int)KeyCode::F3] = glfwGetKeyScancode(GLFW_KEY_F3);						   
	scanCodeMapping[(unsigned int)KeyCode::F4] = glfwGetKeyScancode(GLFW_KEY_F4);						   
	scanCodeMapping[(unsigned int)KeyCode::F5] = glfwGetKeyScancode(GLFW_KEY_F5);						   
	scanCodeMapping[(unsigned int)KeyCode::F6] = glfwGetKeyScancode(GLFW_KEY_F6);						   
	scanCodeMapping[(unsigned int)KeyCode::F7] = glfwGetKeyScancode(GLFW_KEY_F7);						   
	scanCodeMapping[(unsigned int)KeyCode::F8] = glfwGetKeyScancode(GLFW_KEY_F8);						   
	scanCodeMapping[(unsigned int)KeyCode::F9] = glfwGetKeyScancode(GLFW_KEY_F9);						   
	scanCodeMapping[(unsigned int)KeyCode::F10] = glfwGetKeyScancode(GLFW_KEY_F10);						   
	scanCodeMapping[(unsigned int)KeyCode::F11] = glfwGetKeyScancode(GLFW_KEY_F11);						   
	scanCodeMapping[(unsigned int)KeyCode::F12] = glfwGetKeyScancode(GLFW_KEY_F12);						   
	scanCodeMapping[(unsigned int)KeyCode::L_Ctrl] = glfwGetKeyScancode(GLFW_KEY_LEFT_CONTROL);			   
	scanCodeMapping[(unsigned int)KeyCode::L_Shift] = glfwGetKeyScancode(GLFW_KEY_LEFT_SHIFT);			   
	scanCodeMapping[(unsigned int)KeyCode::L_Alt] = glfwGetKeyScancode(GLFW_KEY_LEFT_ALT);				   
	scanCodeMapping[(unsigned int)KeyCode::Caps] = glfwGetKeyScancode(GLFW_KEY_CAPS_LOCK);				   
	scanCodeMapping[(unsigned int)KeyCode::Tab] = glfwGetKeyScancode(GLFW_KEY_TAB);						   
	scanCodeMapping[(unsigned int)KeyCode::Grave_Accent] = glfwGetKeyScancode(GLFW_KEY_GRAVE_ACCENT);	   
	scanCodeMapping[(unsigned int)KeyCode::Esc] = glfwGetKeyScancode(GLFW_KEY_ESCAPE);					   
	scanCodeMapping[(unsigned int)KeyCode::R_Ctrl] = glfwGetKeyScancode(GLFW_KEY_RIGHT_CONTROL);		   
	scanCodeMapping[(unsigned int)KeyCode::R_Shift] = glfwGetKeyScancode(GLFW_KEY_RIGHT_SHIFT);			   
	scanCodeMapping[(unsigned int)KeyCode::R_Alt] = glfwGetKeyScancode(GLFW_KEY_RIGHT_ALT);				   
	scanCodeMapping[(unsigned int)KeyCode::Win] = glfwGetKeyScancode(GLFW_KEY_HOME);					   
	scanCodeMapping[(unsigned int)KeyCode::HashTag] = glfwGetKeyScancode(GLFW_KEY_UNKNOWN);				   
	scanCodeMapping[(unsigned int)KeyCode::Comma] = glfwGetKeyScancode(GLFW_KEY_COMMA);					   
	scanCodeMapping[(unsigned int)KeyCode::Period] = glfwGetKeyScancode(GLFW_KEY_PERIOD);				   
	scanCodeMapping[(unsigned int)KeyCode::Slash] = glfwGetKeyScancode(GLFW_KEY_SLASH);					   
	scanCodeMapping[(unsigned int)KeyCode::Semicolon] = glfwGetKeyScancode(GLFW_KEY_SEMICOLON);			   
	scanCodeMapping[(unsigned int)KeyCode::Minus] = glfwGetKeyScancode(GLFW_KEY_MINUS);					   
	scanCodeMapping[(unsigned int)KeyCode::Equals] = glfwGetKeyScancode(GLFW_KEY_EQUAL);				   
	scanCodeMapping[(unsigned int)KeyCode::BackSpace] = glfwGetKeyScancode(GLFW_KEY_BACKSPACE);			   
	scanCodeMapping[(unsigned int)KeyCode::Enter] = glfwGetKeyScancode(GLFW_KEY_ENTER);					   
	scanCodeMapping[(unsigned int)KeyCode::Space] = glfwGetKeyScancode(GLFW_KEY_SPACE);					   
	scanCodeMapping[(unsigned int)KeyCode::Left_Bracket] = glfwGetKeyScancode(GLFW_KEY_LEFT_BRACKET);	   
	scanCodeMapping[(unsigned int)KeyCode::Right_Bracket] = glfwGetKeyScancode(GLFW_KEY_RIGHT_BRACKET);	   
	scanCodeMapping[(unsigned int)KeyCode::Apostrophe] = glfwGetKeyScancode(GLFW_KEY_APOSTROPHE);		   
	scanCodeMapping[(unsigned int)KeyCode::Arrow_Up] = glfwGetKeyScancode(GLFW_KEY_UP);					   
	scanCodeMapping[(unsigned int)KeyCode::Arrow_Right] = glfwGetKeyScancode(GLFW_KEY_RIGHT);			   
	scanCodeMapping[(unsigned int)KeyCode::Arrow_Down] = glfwGetKeyScancode(GLFW_KEY_DOWN);				   
	scanCodeMapping[(unsigned int)KeyCode::Arrow_Left] = glfwGetKeyScancode(GLFW_KEY_LEFT);				   
	scanCodeMapping[(unsigned int)KeyCode::Delete] = glfwGetKeyScancode(GLFW_KEY_DELETE);				   
	scanCodeMapping[(unsigned int)KeyCode::Insert] = glfwGetKeyScancode(GLFW_KEY_INSERT);				   
	scanCodeMapping[(unsigned int)KeyCode::End] = glfwGetKeyScancode(GLFW_KEY_END);						   
	scanCodeMapping[(unsigned int)KeyCode::Pos1] = glfwGetKeyScancode(GLFW_KEY_PAUSE);					   
	scanCodeMapping[(unsigned int)KeyCode::Page_Up] = glfwGetKeyScancode(GLFW_KEY_PAGE_UP);				   
	scanCodeMapping[(unsigned int)KeyCode::Page_Down] = glfwGetKeyScancode(GLFW_KEY_PAGE_DOWN);			   
	scanCodeMapping[(unsigned int)KeyCode::Menu] = glfwGetKeyScancode(GLFW_KEY_MENU);

	glfwKeyMapping = new int[128];
	glfwKeyMapping[(unsigned int)KeyCode::A] = GLFW_KEY_A;
	glfwKeyMapping[(unsigned int)KeyCode::B] = GLFW_KEY_B;
	glfwKeyMapping[(unsigned int)KeyCode::C] = GLFW_KEY_C;
	glfwKeyMapping[(unsigned int)KeyCode::D] = GLFW_KEY_D;
	glfwKeyMapping[(unsigned int)KeyCode::E] = GLFW_KEY_E;
	glfwKeyMapping[(unsigned int)KeyCode::F] = GLFW_KEY_F;
	glfwKeyMapping[(unsigned int)KeyCode::G] = GLFW_KEY_G;
	glfwKeyMapping[(unsigned int)KeyCode::H] = GLFW_KEY_H;
	glfwKeyMapping[(unsigned int)KeyCode::I] = GLFW_KEY_I;
	glfwKeyMapping[(unsigned int)KeyCode::J] = GLFW_KEY_J;
	glfwKeyMapping[(unsigned int)KeyCode::K] = GLFW_KEY_K;
	glfwKeyMapping[(unsigned int)KeyCode::L] = GLFW_KEY_L;
	glfwKeyMapping[(unsigned int)KeyCode::M] = GLFW_KEY_M;
	glfwKeyMapping[(unsigned int)KeyCode::N] = GLFW_KEY_N;
	glfwKeyMapping[(unsigned int)KeyCode::O] = GLFW_KEY_O;
	glfwKeyMapping[(unsigned int)KeyCode::P] = GLFW_KEY_P;
	glfwKeyMapping[(unsigned int)KeyCode::Q] = GLFW_KEY_Q;
	glfwKeyMapping[(unsigned int)KeyCode::R] = GLFW_KEY_R;
	glfwKeyMapping[(unsigned int)KeyCode::S] = GLFW_KEY_S;
	glfwKeyMapping[(unsigned int)KeyCode::T] = GLFW_KEY_T;
	glfwKeyMapping[(unsigned int)KeyCode::U] = GLFW_KEY_U;
	glfwKeyMapping[(unsigned int)KeyCode::V] = GLFW_KEY_V;
	glfwKeyMapping[(unsigned int)KeyCode::W] = GLFW_KEY_W;
	glfwKeyMapping[(unsigned int)KeyCode::X] = GLFW_KEY_X;
	glfwKeyMapping[(unsigned int)KeyCode::Y] = GLFW_KEY_Y;
	glfwKeyMapping[(unsigned int)KeyCode::Z] = GLFW_KEY_Z;
	glfwKeyMapping[(unsigned int)KeyCode::Num_0] = GLFW_KEY_0;
	glfwKeyMapping[(unsigned int)KeyCode::Num_1] = GLFW_KEY_1;
	glfwKeyMapping[(unsigned int)KeyCode::Num_2] = GLFW_KEY_2;
	glfwKeyMapping[(unsigned int)KeyCode::Num_3] = GLFW_KEY_3;
	glfwKeyMapping[(unsigned int)KeyCode::Num_4] = GLFW_KEY_4;
	glfwKeyMapping[(unsigned int)KeyCode::Num_5] = GLFW_KEY_5;
	glfwKeyMapping[(unsigned int)KeyCode::Num_6] = GLFW_KEY_6;
	glfwKeyMapping[(unsigned int)KeyCode::Num_7] = GLFW_KEY_7;
	glfwKeyMapping[(unsigned int)KeyCode::Num_8] = GLFW_KEY_8;
	glfwKeyMapping[(unsigned int)KeyCode::Num_9] = GLFW_KEY_9;
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_0] = GLFW_KEY_KP_0;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_1] = GLFW_KEY_KP_1;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_2] = GLFW_KEY_KP_2;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_3] = GLFW_KEY_KP_3;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_4] = GLFW_KEY_KP_4;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_5] = GLFW_KEY_KP_5;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_6] = GLFW_KEY_KP_6;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_7] = GLFW_KEY_KP_7;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_8] = GLFW_KEY_KP_8;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_9] = GLFW_KEY_KP_9;				   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Period] = GLFW_KEY_KP_DECIMAL;	   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Enter] = GLFW_KEY_KP_ENTER;		   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Plus] = GLFW_KEY_KP_ADD;			   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Minus] = GLFW_KEY_KP_SUBTRACT;	   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Mul] = GLFW_KEY_KP_MULTIPLY;		   
	glfwKeyMapping[(unsigned int)KeyCode::NumPad_Div] = GLFW_KEY_KP_DIVIDE;		   
	glfwKeyMapping[(unsigned int)KeyCode::F1] = GLFW_KEY_F1;						   
	glfwKeyMapping[(unsigned int)KeyCode::F2] = GLFW_KEY_F2;						   
	glfwKeyMapping[(unsigned int)KeyCode::F3] = GLFW_KEY_F3;						   
	glfwKeyMapping[(unsigned int)KeyCode::F4] = GLFW_KEY_F4;						   
	glfwKeyMapping[(unsigned int)KeyCode::F5] = GLFW_KEY_F5;						   
	glfwKeyMapping[(unsigned int)KeyCode::F6] = GLFW_KEY_F6;						   
	glfwKeyMapping[(unsigned int)KeyCode::F7] = GLFW_KEY_F7;						   
	glfwKeyMapping[(unsigned int)KeyCode::F8] = GLFW_KEY_F8;						   
	glfwKeyMapping[(unsigned int)KeyCode::F9] = GLFW_KEY_F9;						   
	glfwKeyMapping[(unsigned int)KeyCode::F10] = GLFW_KEY_F10;						   
	glfwKeyMapping[(unsigned int)KeyCode::F11] = GLFW_KEY_F11;						   
	glfwKeyMapping[(unsigned int)KeyCode::F12] = GLFW_KEY_F12;						   
	glfwKeyMapping[(unsigned int)KeyCode::L_Ctrl] = GLFW_KEY_LEFT_CONTROL;			   
	glfwKeyMapping[(unsigned int)KeyCode::L_Shift] = GLFW_KEY_LEFT_SHIFT;			   
	glfwKeyMapping[(unsigned int)KeyCode::L_Alt] = GLFW_KEY_LEFT_ALT;				   
	glfwKeyMapping[(unsigned int)KeyCode::Caps] = GLFW_KEY_CAPS_LOCK;				   
	glfwKeyMapping[(unsigned int)KeyCode::Tab] = GLFW_KEY_TAB;						   
	glfwKeyMapping[(unsigned int)KeyCode::Grave_Accent] = GLFW_KEY_GRAVE_ACCENT;	   
	glfwKeyMapping[(unsigned int)KeyCode::Esc] = GLFW_KEY_ESCAPE;					   
	glfwKeyMapping[(unsigned int)KeyCode::R_Ctrl] = GLFW_KEY_RIGHT_CONTROL;		   
	glfwKeyMapping[(unsigned int)KeyCode::R_Shift] = GLFW_KEY_RIGHT_SHIFT;			   
	glfwKeyMapping[(unsigned int)KeyCode::R_Alt] = GLFW_KEY_RIGHT_ALT;				   
	glfwKeyMapping[(unsigned int)KeyCode::Win] = GLFW_KEY_HOME;					   
	glfwKeyMapping[(unsigned int)KeyCode::HashTag] = GLFW_KEY_UNKNOWN;				   
	glfwKeyMapping[(unsigned int)KeyCode::Comma] = GLFW_KEY_COMMA;					   
	glfwKeyMapping[(unsigned int)KeyCode::Period] = GLFW_KEY_PERIOD;				   
	glfwKeyMapping[(unsigned int)KeyCode::Slash] = GLFW_KEY_SLASH;					   
	glfwKeyMapping[(unsigned int)KeyCode::Semicolon] = GLFW_KEY_SEMICOLON;			   
	glfwKeyMapping[(unsigned int)KeyCode::Minus] = GLFW_KEY_MINUS;					   
	glfwKeyMapping[(unsigned int)KeyCode::Equals] = GLFW_KEY_EQUAL;				   
	glfwKeyMapping[(unsigned int)KeyCode::BackSpace] = GLFW_KEY_BACKSPACE;			   
	glfwKeyMapping[(unsigned int)KeyCode::Enter] = GLFW_KEY_ENTER;					   
	glfwKeyMapping[(unsigned int)KeyCode::Space] = GLFW_KEY_SPACE;					   
	glfwKeyMapping[(unsigned int)KeyCode::Left_Bracket] = GLFW_KEY_LEFT_BRACKET;	   
	glfwKeyMapping[(unsigned int)KeyCode::Right_Bracket] = GLFW_KEY_RIGHT_BRACKET;	   
	glfwKeyMapping[(unsigned int)KeyCode::Apostrophe] = GLFW_KEY_APOSTROPHE;		   
	glfwKeyMapping[(unsigned int)KeyCode::Arrow_Up] = GLFW_KEY_UP;					   
	glfwKeyMapping[(unsigned int)KeyCode::Arrow_Right] = GLFW_KEY_RIGHT;			   
	glfwKeyMapping[(unsigned int)KeyCode::Arrow_Down] = GLFW_KEY_DOWN;				   
	glfwKeyMapping[(unsigned int)KeyCode::Arrow_Left] = GLFW_KEY_LEFT;				   
	glfwKeyMapping[(unsigned int)KeyCode::Delete] = GLFW_KEY_DELETE;				   
	glfwKeyMapping[(unsigned int)KeyCode::Insert] = GLFW_KEY_INSERT;				   
	glfwKeyMapping[(unsigned int)KeyCode::End] = GLFW_KEY_END;						   
	glfwKeyMapping[(unsigned int)KeyCode::Pos1] = GLFW_KEY_PAUSE;					   
	glfwKeyMapping[(unsigned int)KeyCode::Page_Up] = GLFW_KEY_PAGE_UP;				   
	glfwKeyMapping[(unsigned int)KeyCode::Page_Down] = GLFW_KEY_PAGE_DOWN;			   
	glfwKeyMapping[(unsigned int)KeyCode::Menu] = GLFW_KEY_MENU;					   


	glfwSetKeyCallback(Window::GetInstance()->GLFW_GetWindow(), KeyPressCallcack);
	glfwSetCharCallback(Window::GetInstance()->GLFW_GetWindow(), TextInputCallback);
}


void Keyboard::SendKeyEvents()
{
	for (auto func : registered_GKGSF_Functions) {
		func._Myfirst._Val(keyCodeMapping[receivedScanCode], (KeyState)receivedAction);
	}
}


void Keyboard::SendTextEvents()
{
	for (auto func : registered_TICF_Functions) {
		func._Myfirst._Val(receivedTextInput);
	}
}


void Keyboard::KeyPressJoinThread()
{
	if (keyInputThread.joinable()) {
		keyInputThread.join();
	}

	auto funcIt = std::begin(registered_GKGSF_Functions);
	while (funcIt != registered_GKGSF_Functions.begin()) {
		for (auto delFunc : toUnregister_GKGSF_Functions) {
			if (funcIt->_Get_rest()._Myfirst._Val == delFunc) {
				funcIt = registered_GKGSF_Functions.erase(funcIt);
			}
			else {
				++funcIt;
			}
		}
	}

	toUnregister_GKGSF_Functions.clear();
}


void Keyboard::TextInputJoinThread() {
	if (textInputThread.joinable()) {
		textInputThread.join();
	}

	auto funcIt = std::begin(registered_TICF_Functions);
	while (funcIt != registered_TICF_Functions.begin()) {
		for (auto delFunc : toUnregister_TICF_Functions) {
			if (funcIt->_Get_rest()._Myfirst._Val == delFunc) {
				funcIt = registered_TICF_Functions.erase(funcIt);
			}
			else {
				++funcIt;
			}
		}
	}

	toUnregister_TICF_Functions.clear();
}


int Keyboard::RegisterOnAnyKeyCallback(GetKeyGetStateFunc callback)
{
	KeyPressJoinThread();
	registered_GKGSF_Functions.push_back(std::tuple<GetKeyGetStateFunc, int>(callback, ++latestKeyHandle));
	return latestKeyHandle;
}


void Keyboard::UnregisterOnAnyKeyCallback(int handle)
{
	toUnregister_GKGSF_Functions.push_back(handle);
}


void Keyboard::KeyPressCallcack(GLFWwindow* window, int key, int scanCode, int action, int mods)
{	
	if (!voidKeyPressCallback) {
		receivedScanCode = scanCode;
		receivedAction = action;
		KeyPressJoinThread();
		//keyInputThread = std::thread(SendKeyEvents);
		SendKeyEvents();
	}
}

bool Keyboard::GetKey(KeyCode keyCode)
{
	return glfwGetKey(Window::GetInstance()->GLFW_GetWindow(), glfwKeyMapping[(unsigned int)keyCode]);
}


void Keyboard::TextInputCallback(GLFWwindow* window, unsigned int codePoint)
{
	if (!voidTextInputCallback) {
		receivedTextInput = codePoint;
		TextInputJoinThread();
		//textInputThread = std::thread(SendTextEvents);
		SendTextEvents();
	}
}


int Keyboard::RegisterOnTextInputCallback(TextInputCharFunc callback) 
{
	TextInputJoinThread();
	registered_TICF_Functions.push_back(std::tuple<TextInputCharFunc, int>(callback, ++latestTextHandle));
	return latestTextHandle;
}


void Keyboard::UnregisterOnTextInputCallback(int handle) 
{
	toUnregister_TICF_Functions.push_back(handle);
}


void Keyboard::JoinInputThreads() {
	KeyPressJoinThread();
	TextInputJoinThread();	
}
