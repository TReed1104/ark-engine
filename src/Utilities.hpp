#ifndef ARKENGINE_UTILITIES_HPP_
#define ARKENGINE_UTILITIES_HPP_
#include <string>
#include <vector>
#include <filesystem>

namespace FileSystem = std::experimental::filesystem;

class FileSystemUtilities {
public:
	static std::vector<std::string> GetFileList(const std::string& directoryPath) {
		// Finds all the files within the given Directory.
		std::vector<std::string> fileList;

		for (FileSystem::path currentEntry : FileSystem::directory_iterator(directoryPath)) {
			fileList.push_back(currentEntry.string());
		}
		return fileList;
	}
	static std::vector<std::string> GetFileListRecurssively(const std::string& directoryPath) {
		// Finds all the files within the given Directory and any child directories.
		std::vector<std::string> fileList;
		for (FileSystem::path currentEntry : FileSystem::recursive_directory_iterator(directoryPath)) {
			fileList.push_back(currentEntry.string());
		}
		return fileList;
	}

private:

};

class StringUtilities {
public:
	static std::vector<std::string> Split(const std::string& stringToSplit, const char& splitToken) {
		// Splits a string using the given splitToken, E.g. ""The.Cat.Sat.On.The.Mat" splits with token '.' into Array[6] = {The, Cat, Sat, On, The, Mat};

		std::vector<std::string> splitString;	// Stores the split sections of string for the return.
		std::string currentSplit = "";			// Stores the current section being split off.

		int sizeOfStringArray = stringToSplit.size();			// .Size() of a const so it never changes and we store it once.
		for (unsigned int i = 0; i < sizeOfStringArray; i++) {
			char currentChar = stringToSplit.at(i);
			if (currentChar == splitToken) {
				splitString.push_back(currentSplit);
				currentSplit = "";
			}
			else {
				currentSplit += currentChar;
			}

			if (i == sizeOfStringArray - 1 && currentChar != splitToken) {
				// Catches the final section of string as there might not be a follow up split token.
				splitString.push_back(currentSplit);
			}
		}

		return splitString;
	}
};


#endif