#ifndef ARKENGINE_KEYBINDINGS_H_
#define ARKENGINE_KEYBINDINGS_H_

#include <map>
#include "Keyboard.h"

struct Keybind {
	std::string id;
	std::string friendlyName;
	Keyboard::Keys bindingValue;
};

class KeybindingHandler {
public:
	KeybindingHandler();
	~KeybindingHandler();

	Keyboard::Keys GetKeybind(const std::string& bindingName);
	bool AddKeybinding(const Keybind& newBinding);

private:
	std::map<std::string, Keybind> bindings;
	std::map<std::string, std::string> friendlyNames;

};

#endif
