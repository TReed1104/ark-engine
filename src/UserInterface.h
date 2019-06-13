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

	class Button {
	public:
		Button(std::string name);
		~Button();

		const std::string GetName(void);
		void(*OnClick)();
		void(*MouseOver)();
		void Update(const float& deltaTime);
		void Draw(void);

	private:
		std::string name;
		int indexOfShader = -1;
		Texture* texture = nullptr;
		Model* model = nullptr;
		glm::vec2 dimensions;
		glm::vec2 position;
	};


	UserInterface(const std::string& filePath = "NOT LOADED");
	~UserInterface();

	const std::string GetName(void);
	const glm::vec2 GetPosition(void);
	void SetPosition(const glm::vec2& newPosition);
	const bool IsLoaded(void);
	const bool IsActive(void);
	void SetActiveState(const bool& newState);

	void Update(const float& deltaTime);
	void Draw(void);

private:
	JsonFile* configFile = nullptr;
	std::string name = "";
	bool isLoaded = false;
	bool isActive = false;
	glm::ivec2 dimensions;
	glm::vec3 position;
	int indexOfShader = -1;
	Texture* texture = nullptr;
	Model* model = nullptr;

	std::vector<Button*> buttonRegister;
	std::vector<TextObject*> textRegister;

	bool Load(const std::string& configFilePath);
};

#endif
