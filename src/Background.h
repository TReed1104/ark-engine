#ifndef ARKENGINE_BACKGROUND_H_
#define ARKENGINE_BACKGROUND_H_

#include <string>

class Background {
public:

	Background();
	~Background();

	const std::string GetName(void);

private:
	std::string name;
};

#endif