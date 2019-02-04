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

		// Set States
		void SetDebuggingState(const bool& isEnabled) {
			this->isDebuggerEnabled = isEnabled;

		}
		void SetLoggingState(const bool& isEnabled) {
			this->isLoggingEnabled = isEnabled;
		}

		// Get States
		const bool GetDebuggingState() {
			return isDebuggerEnabled;
		}
		const bool GetLoggingState() {
			return isLoggingEnabled;
		}

	private:
		// Private Variables
		bool isDebuggerEnabled = false;
		bool isLoggingEnabled = false;
		std::string loggingFilePath = "";

	};
};

#endif
