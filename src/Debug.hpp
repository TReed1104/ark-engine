#ifndef CPP_DEBUG_HPP_
#define CPP_DEBUG_HPP_

#include <iostream>
#include <string>

namespace Debug {
	static bool isDebuggingEnabled = false;
	static bool isLoggingEnabled = false;
	static std::string loggingFileName = "";

	static void Log(const std::string& output) {
		// TODO: Write line to output file
	}
	static void Print(const std::string& output) {
		if (isDebuggingEnabled) {
			std::cout << output << std::endl;
			if (isLoggingEnabled) {
				Debug::Log(output);
			}
		}
	}
};

#endif
