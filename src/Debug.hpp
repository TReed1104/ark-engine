#ifndef CPP_DEBUG_HPP_
#define CPP_DEBUG_HPP_

#include <iostream>
#include <string>

namespace Debug {
	class Debugger {
	public:

		// Constructors
		Debugger() {

		}
		~Debugger() {

		}

		// Outputs
		void LogLine(const std::string& output) {
			// TODO: Write line to output file
		}
		void WriteLine(const std::string& output) {
			if (isDebuggerEnabled) {
				std::cout << output << std::endl;
				if (isLoggingEnabled) {
					LogLine(output);
				}
			}
		}

		// Get States
		const std::string GetName() {
			return name;
		}
		const bool GetDebuggingState() {
			return isDebuggerEnabled;
		}
		const bool GetLoggingState() {
			return isLoggingEnabled;
		}
		const std::string GetLoggingFilePath() {
			return loggingFilePath;
		}

		// Set States
		void SetName(const std::string& newName) {
			this->name = newName;
		}
		void SetDebuggingState(const bool& isEnabled) {
			this->isDebuggerEnabled = isEnabled;
		}
		void SetLoggingState(const bool& isEnabled) {
			this->isLoggingEnabled = isEnabled;
		}
		void SetLoggingFilePath(const std::string& newFilePath) {
			this->loggingFilePath = newFilePath;
		}
		
	private:
		// Private Variables
		std::string name;
		bool isDebuggerEnabled = false;
		bool isLoggingEnabled = false;
		std::string loggingFilePath = "";

	};
};

#endif
