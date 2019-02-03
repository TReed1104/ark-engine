#ifndef CPP_DEBUG_HPP_
#define CPP_DEBUG_HPP_

#include <iostream>
#include <string>

class Debug {
	static bool isDebuggingEnabled;
	static bool isLoggingEnabled;
	static std::string loggingFileName;

	static void Print(const std::string& output) {
		if (isDebuggingEnabled) {
			std::cout << output << std::endl;
			if (isLoggingEnabled) {
				Log(output);
			}
		}
	}
	static void Log(const std::string& output) {
		// TODO: Write line to output file
	}
};

#endif
