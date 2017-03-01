#include "LuaScript.h"

LuaScript::LuaScript(const std::string& scriptName) {
	L = luaL_newstate();
	if (luaL_loadfile(L, scriptName.c_str()) || lua_pcall(L, 0, 0, 0)) {
		std::cout << "Error: script not loaded (" << scriptName << ")" << std::endl;
		L = 0;
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
void LuaScript::Get(const std::string & variableName) {

}
void LuaScript::Run() {

}
