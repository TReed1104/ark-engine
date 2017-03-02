#include "LuaScript.h"

LuaScript::LuaScript(const std::string& scriptName) {
	L = luaL_newstate();
	// Makes sure the file is loaded.
	if (luaL_loadfile(L, scriptName.c_str()) || lua_pcall(L, 0, 0, 0)) {
		std::cout << "Error: script not loaded (" << scriptName << ")" << std::endl;
		L = 0;
	}
	if (L) {
		luaL_openlibs(L);
	}
}
LuaScript::~LuaScript() {
	if (L) {
		lua_close(L);
	}
}

void LuaScript::OutputError(const std::string & variableName, const std::string & error) {
	std::cout << "Error: Unable to get " << variableName << " due to error: " << error << std::endl;
}
bool LuaScript::HandleLuaStack(const std::string & variableName) {
	currentLevel = 0;
	std::string currentVariableLevel = "";

	// Loop through each character if the variable and find out what stack level the required variable is at.
	for (unsigned int i = 0; i < variableName.size(); i++) {
		// If the current character us a '.' marking the next level in the stack of the variable
		if (variableName.at(i) == '.') {
			// If we are at the root of the stack, Call get global as it is the root of a variable stack.
			if (currentLevel == 0) {
				lua_getglobal(L, currentVariableLevel.c_str());
			}
			else { // If we are not at the root, call get field as all subsequent parts of the variable are fields not globals.
				lua_getfield(L, -1, currentVariableLevel.c_str());
			}

			// Check if a variable was found at the current stack.
			if (lua_isnil(L, -1)) {
				// Not found, kick out an error and return false from the function.
				OutputError(variableName, currentVariableLevel + " is not defined");
				return false;
			}
			else {
				// Level found, move to the next level.
				currentVariableLevel = "";
				currentLevel++;
			}
		}
		else {	// If its not a hierarchy marker, add the 
			currentVariableLevel += variableName.at(i);
		}
	}

	// If we are at the root of the stack, Call get global as it is the root of a variable stack.
	if (currentLevel == 0) {
		lua_getglobal(L, currentVariableLevel.c_str());
	}
	else { // If we are not at the root, call get field as all subsequent parts of the variable are fields not globals.
		lua_getfield(L, -1, currentVariableLevel.c_str());
	}
	// Check if a variable was found at the current stack.
	if (lua_isnil(L, -1)) {
		// Not found, kick out an error and return false from the function.
		OutputError(variableName, currentVariableLevel + " is not defined");
		return false;
	}

	return true;
}
void LuaScript::Run() {

}
