#ifndef ARKENGINE_KEYBINDINGS_H_
#define ARKENGINE_KEYBINDINGS_H_

#include <map>
#include "Keyboard.h"

struct Keybind {
	std::string id;
	std::string friendlyName;
	Keyboard::Keys bindingValue;
};

#endif
