#ifndef ARKENGINE_UTILITIES_HPP_
#define ARKENGINE_UTILITIES_HPP_
#include <string>
#include <vector>

class Utilities
{
public:
	Utilities() {}
	~Utilities() {}

	static std::vector<std::string> StringSplit(const std::string& stringToSplit, const char& splitToken) {
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
	static std::vector<std::string> GetFilesFromDirectory() {
		std::vector<std::string> listOfFiles;

		

		return listOfFiles;
	}

private:

};

#endif