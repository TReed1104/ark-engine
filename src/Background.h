#ifndef ARKENGINE_BACKGROUND_H_
#define ARKENGINE_BACKGROUND_H_

#include <string>

class Background {
public:

	Background(const std::string name);
	~Background();

	const std::string GetName(void);

private:
	std::string name;
};

#endif