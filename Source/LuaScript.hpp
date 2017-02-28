#ifndef ARKENGINE_LUASCRIPT_HPP_
#define ARKENGINE_LUASCRIPT_HPP_

#include <string>
#include <vector>
#include <iostream>

class LuaScript
{
public:
	LuaScript(const std::string& fileName);
	~LuaScript();

	template<typename T>
	T Get(const std::string& variableName);

private:

};

LuaScript::LuaScript(const std::string& fileName) {

}
LuaScript::~LuaScript() {

}
template<typename T>
T LuaScript::Get(const std::string & variableName) {
	return 0;
}
#endif