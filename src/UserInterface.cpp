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
		}
	}
	else {
		configFile = nullptr;
	}
}
UserInterface::~UserInterface() {
}

const std::string UserInterface::GetName(void) {
	return this->name;
}
void UserInterface::Update(const float& deltaTime) {

}
void UserInterface::Draw(void) {

}