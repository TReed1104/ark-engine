#ifndef ARKENGINE_BACKGROUND_H_
#define ARKENGINE_BACKGROUND_H_

#include <string>

class Engine;

class Background {
public:
	static Engine* Engine_Pointer;

	Background(const std::string name);
	~Background();

	void Update(const float& deltaTime);
	void Draw(void);
	const std::string GetName(void);

private:
	std::string name;
};

#endif