#ifndef CPP_DEBUG_HPP_
#define CPP_DEBUG_HPP_

#include <iostream>
#include <fstream>
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
			// If the output file has been initialised
			if (outputFile != nullptr) {
				// If the output file is open
				if (outputFile->is_open()) {
					outputFile->flush();	// Flush the data in the output file stream
					outputFile->close();	// Close the output file stream
				}
				delete outputFile;	// Clean up our manually allocated memory for the output file stream
			}
		}

		// Outputs
		void LogLine(const std::string& output) {
			// Check we've set the file name
			if (loggingFilePath == "") {
				std::cout << "ERROR: Logging file name for Debugger - " << name << " has not been configured" << std::endl;
				return;
			}
			// If the Output file hasn't been initialised, initialise it
			if (outputFile == nullptr) {
				outputFile = new std::ofstream();
			}
			// See if the file has been opened
			if (!outputFile->is_open()) {
				// TODO: change name of file to also use the date+time
				outputFile->open("logs/" + loggingFilePath);
			}
			// Write the output string to the output filestream
			*(outputFile) << output << std::endl;
		}
		void WriteLine(const std::string& output) {
			// Check the debugger is enabled
			if (isDebuggerEnabled) {
				// Write to the console
				std::cout << output << std::endl;
				// Check we are configured to log our outputs
				if (isLoggingEnabled) {
					// Try and log the output
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
		std::ofstream* outputFile = nullptr;

	};
};

#endif
