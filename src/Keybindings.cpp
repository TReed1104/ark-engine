#include "Keybindings.h"

KeybindingHandler::KeybindingHandler() {
}
KeybindingHandler::~KeybindingHandler() {
}

Keyboard::Keys KeybindingHandler::GetKeybind(const std::string & bindingName) {
	return Keyboard::Keys();
}
bool KeybindingHandler::AddKeybinding(const Keybind& newBinding) {
	return false;
}
