#ifndef ARKENGINE_KEYBOARD_H_
#define ARKENGINE_KEYBOARD_H_

#include <map>
#include "SDL.h"

class Engine;

class Keyboard
{
public:
	static Engine* Engine_Pointer;
	enum Keys { num0, num1, num2, num3, num4, num5, num6, num7, num8, num9, 
				a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, 
				A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, 
				Escape, Space, LeftCtrl, LeftAlt, LeftShift, RightCtrl, RightAlt, RightShift };

	Keyboard();
	~Keyboard();

	bool GetKeyState(const Keys& key);
	void UpdateKeyStates_Up(const SDL_KeyboardEvent& keyboardEvent);
	void UpdateKeyStates_Down(const SDL_KeyboardEvent& keyboardEvent);

private:
	std::map<std::string, bool> upperCaseKeyStates;
	std::map<std::string, bool> lowerCaseKeyStates;
	std::map<std::string, bool> numberKeyStates;
	std::map<std::string, bool> specialKeyStates;

};

#endif