#ifndef ARKENGINE_USERINTERFACE_H_
#define ARKENGINE_USERINTERFACE_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Texture.h"
#include "Animation.h"
#include "BoundingBox.h"
#include "JsonParser.hpp"
#include "Font.h"
#include "TextObject.h"


class Engine;

class UserInterface {
public:
	static Engine* Engine_Pointer;

	UserInterface(const std::string& name, const std::string& configFile);
	~UserInterface();

	const std::string GetName(void);
	void Update(const float& deltaTime);
	void Draw(void);

private:
	JsonFile* configFile = nullptr;
	std::string name;
};

#endif
