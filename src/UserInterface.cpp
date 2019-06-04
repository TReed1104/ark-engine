#include "UserInterface.h"
#include "Engine.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::Button::Button(std::string name) {

}
UserInterface::Button::~Button() {

}

const std::string UserInterface::Button::GetName(void) {
	return name;
}
void UserInterface::Button::Update(const float & deltaTime) {
	// Check the OnClick is set
	if (OnClick != nullptr) {
		OnClick();
	}
	// Check the MouseOver is set
	if (MouseOver != nullptr) {
		MouseOver();
	}
}

UserInterface::UserInterface(const std::string& filePath) {
	if (filePath != "NOT LOADED") {
		configFile = new JsonFile(filePath);
		if (configFile->IsLoaded()) {
			name = configFile->Get<std::string>("interface.id");
			dimensions = glm::ivec2(configFile->Get<int>("interface.dimensions.width"), configFile->Get<int>("interface.dimensions.height"));
			texture = new Texture(name, configFile->Get<std::string>("interface.texture"), true, false);

			if (texture->IsLoaded()) {

			}
			else {
				isLoaded = false;
			}
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> UserInteface failed to load Config File: " + filePath);
			isLoaded = false;
		}
	}
	else {
		configFile = nullptr;
		isLoaded = false;
	}
}
UserInterface::~UserInterface() {
	// Clean up the created buttons
	size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		delete buttonRegister[i];
	}

	// If the texture has been loaded, delete it
	if (texture != nullptr) {
		delete texture;
	}

	// If the model has been generated, delete it
	if (model != nullptr) {
		delete model;
	}
}

const std::string UserInterface::GetName(void) {
	return this->name;
}
const bool UserInterface::IsLoaded(void) {
	return isLoaded;
}
void UserInterface::Update(const float& deltaTime) {
	size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		buttonRegister[i]->Update(deltaTime);
	}
}
void UserInterface::Draw(void) {

}
