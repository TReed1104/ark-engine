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

	void OutputError(const std::string& variableName, const std::string& error);
	void Get(const std::string& variableName);
	void Run();

private:
	lua_State* L;
};

#endif