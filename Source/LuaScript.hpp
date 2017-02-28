#ifndef ARKENGINE_LUASCRIPT_HPP_
#define ARKENGINE_LUASCRIPT_HPP_

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
	LuaScript(const std::string& fileName) {

	}
	~LuaScript() {

	}

	template<typename T>
	T Get(const std::string& variableName) {
		return 0;
	}

private:

};
#endif