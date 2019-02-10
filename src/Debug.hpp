#ifndef CPP_DEBUG_HPP_
#define CPP_DEBUG_HPP_

#include <iostream>
#include <string>

namespace Debug {
	class Debugger {
	public:

		// Constructors
		Debugger(const std::string& name = "", const bool& isDebuggerEnabled = false, const bool& isLoggingEnabled = false, const std::string& loggingFilePath = "") {
			this->name = name;
			this->isDebuggerEnabled = isDebuggerEnabled;
			this->isLoggingEnabled = isLoggingEnabled;
			this->loggingFilePath = loggingFilePath;
		}
		~Debugger() {

		}

		// Outputs
		void LogLine(const std::string& output) {
			// TODO: Write line to output file

			// Check the logging directory exists
				// If it does, check if our file exists
					// If log file is not present, create the log file
			
			// Log the output line
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
