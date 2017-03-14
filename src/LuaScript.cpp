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
void LuaScript::Clean() {
	// Clean up the stack
	int n = lua_gettop(L);
	lua_pop(L, n);
}
bool LuaScript::HandleLuaStack(const std::string & variableName) {
	// Split the variableName into its hierarchy.
	std::vector<std::string> variableNameSplit = Utilities::StringSplit(variableName, '.');
	int numberOfLevels = variableNameSplit.size();	// Store the size and .size() recalculates the size each call and the size is constant here.
	for (int i = 0; i < numberOfLevels; i++) {
		if (i == 0) {
			// If we are at the head of the stack, get global.
			lua_getglobal(L, variableNameSplit[i].c_str());
		}
		else {
			// If we are not at the head, get the accessible field with the current level of the stack.
			lua_getfield(L, -1, variableNameSplit[i].c_str());
		}
		if (lua_isnil(L, -1)) {
			// Not found, kick out an error and return false from the function.
			OutputError(variableName, i + " is not defined");
			return false;
		}
	}
	return true;
}
void LuaScript::RunScript() {

}
