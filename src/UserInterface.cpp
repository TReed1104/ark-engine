#include "UserInterface.h"
#include "Engine.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::UserInterface(const std::string& filePath) {
	if (filePath != "NOT LOADED") {
		configFile = new JsonFile(filePath);
		if (configFile->IsLoaded()) {
			name = configFile->Get<std::string>("interface.id");
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
}

const std::string UserInterface::GetName(void) {
	return this->name;
}
const bool UserInterface::IsLoaded(void) {
	return isLoaded;
}
void UserInterface::Update(const float& deltaTime) {

}
void UserInterface::Draw(void) {

}