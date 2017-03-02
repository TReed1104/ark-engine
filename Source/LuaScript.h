#ifndef ARKENGINE_LUASCRIPT_H_
#define ARKENGINE_LUASCRIPT_H_

#include <string>
#include <vector>
#include <iostream>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class LuaScript
{
public:
	LuaScript(const std::string& scriptName);
	~LuaScript();

	template<typename T> T Get(const std::string& variableName);
	void Run();

private:
	lua_State* L;
	int currentLevel;

	void OutputError(const std::string& variableName, const std::string& error);
	template<typename T> T GetDefaultValue();
	template<typename T> T GetValueFromLua(const std::string& variableName);
	bool HandleLuaStack(const std::string& variableName);
};

// This function handles getting variables from the LuaScript.
template<typename T> T LuaScript::Get(const std::string& variableName) {
	// Check the script is loaded and accessible.
	if (!L) {
		OutputError(variableName, "Could not load lua script");
		return GetDefaultValue<T>();
	}

	T luaValue;	// Stores the value from the lua script.
	if (HandleLuaStack(variableName)) {
		luaValue = GetValueFromLua<T>(variableName);
	}
	else {
		luaValue = GetDefaultValue<T>();
	}

	lua_pop(L, currentLevel + 1);
	return luaValue;
}
// Functions returning a value when things go wrong.
template<typename T> inline T LuaScript::GetDefaultValue() {
	return 0;
}
template<> inline std::string LuaScript::GetDefaultValue<std::string>() {
	return "null";
}
// Functions handling the conversion from the lua state into the values we want to use.
template<typename T> T LuaScript::GetValueFromLua(const std::string & variableName) {
	return 0;
}
template<> inline int LuaScript::GetValueFromLua(const std::string & variableName) {
	if (!lua_isnumber(L, -1)) {
		OutputError(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}
template<> inline float LuaScript::GetValueFromLua(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		OutputError(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}
template<> inline bool LuaScript::GetValueFromLua(const std::string& variableName) {
	return (bool)lua_toboolean(L, -1);
}

#endif