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
		std::vector<std::string> splitString;

		std::string currentSplit = "";
		int sizeOfStringArray = stringToSplit.size();
		for (unsigned int i = 0; i < sizeOfStringArray; i++) {
			if (stringToSplit.at(i) == splitToken) {
				splitString.push_back(currentSplit);
				currentSplit = "";
			}
			else {
				currentSplit += stringToSplit.at(i);
			}
			if (i == sizeOfStringArray - 1) {
				splitString.push_back(currentSplit);
			}
		}

		return splitString;
	}

private:

};

#endif