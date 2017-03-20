#ifndef ARKENGINE_KEYBOARD_H_
#define ARKENGINE_KEYBOARD_H_

#include <map>
#include "SDL.h"

class Engine;

class Keyboard
{
public:
	static Engine* Engine_Pointer;
	enum NumberKeys { num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};
	enum LowerKeys { a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z};
	enum CapitalKeys { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
	enum SymbolKeys {Escape, Space, };

	Keyboard();
	~Keyboard();

	bool GetKeyState(const LowerKeys& key);
	bool GetKeyState(const CapitalKeys& key);
	bool GetKeyState(const NumberKeys& key);
	bool GetKeyState(const SymbolKeys& key);
	void UpdateKetStates(const SDL_KeyboardEvent& keyboardEvent);

private:
	std::map<std::string, bool> capitalKeyStates;
	std::map<std::string, bool> lowerKeyStates;
	std::map<std::string, bool> numberKeyStates;
	std::map<std::string, bool> symbolKeyStates;

};

#endif