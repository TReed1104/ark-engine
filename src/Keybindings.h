#ifndef ARKENGINE_KEYBINDINGS_H_
#define ARKENGINE_KEYBINDINGS_H_

#include <iostream>
#include <string>
#include <map>
#include "Keyboard.h"

class Engine;

struct Keybind {
	std::string id;
	std::string friendlyName;
	Keyboard::Keys bindingValue;
};

class KeybindingHandler {
public:
	static Engine* Engine_Pointer;

	KeybindingHandler();
	~KeybindingHandler();

	Keyboard::Keys GetKeybind(const std::string& bindingFriendlyName);
	bool AddKeybinding(const Keybind& newBinding);

private:
	std::map<std::string, Keybind> keybinds;
	std::map<std::string, std::string> friendlyNames;

};

#endif
