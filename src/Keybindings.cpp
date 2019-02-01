#include "Keybindings.h"

KeybindingHandler::KeybindingHandler() {
}
KeybindingHandler::~KeybindingHandler() {
}

Keyboard::Keys KeybindingHandler::GetKeybind(const std::string & bindingName) {
	return Keyboard::Keys();
}
bool KeybindingHandler::AddKeybinding(const Keybind& newBinding) {
	// Check fi the binding exists
	if (bindings.find(newBinding.id) == bindings.end()) {
		// If it doesn't already exist, add it
		bindings[newBinding.id] = newBinding;
		return true;
	}
	else {
		std::cout << ">>>> Error - Binding already exists" << std::endl;
		return false;
	}
}
