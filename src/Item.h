#ifndef ARKENGINE_ITEM_H_
#define ARKENGINE_ITEM_H_
#include "GameObject.h"

class Item : public GameObject {
public:
	Item(const std::string& filePath);
	~Item(void);

	std::map<std::string, std::string> ExportDataForBinding(void);
private:

protected:
	void LoadAnimations(void);
	void AnimationStateHandler(void);
	void AnimationIndexHandler(void);
};
#endif